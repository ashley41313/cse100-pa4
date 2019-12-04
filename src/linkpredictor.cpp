/*
 * pathfinder.cpp
 * Author: Ashley Eckert
 * Date:   December 3, 2019
 *
 * This file is meant to take in command inputs and choose wether
 * to use BFS search of Dijkstras algorithm to find a path between
 * two actors.
 * it calls ActorGraph class functions to do this.
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#define SECONDLAYER 2
#define FIRSTLAYER 1
#define MAX 4

#include <vector>
#include "ActorGraph.hpp"

using namespace std;

/*this function does TOO much. It calls an actorgraph function
 * to get the first two layers of the BFS search, then it calls a
 *function on those vectors to find distances between each, then
 *orders THAT via a Prioirity queue*/
void linkpredictor(vector<string> query, const char* inFilename,
                   ostream& firstOutfile, ostream& secondOutfile) {
    /*create the graph from the infile*/
    ActorGraph graph;
    graph.loadFromFile(inFilename, 0);

    for (int i = 0; i < query.size(); i++) {
        string queryActor = query[i];

        /*get the first/second layer of BFS*/
        vector<string> v1 = graph.partialBFS(queryActor, FIRSTLAYER);
        vector<string> v2 = graph.partialBFS(queryActor, SECONDLAYER);

        /*PART 2: CALCULATE THEIR PRIORITIES*/
        for (int i = 0; i < v1.size(); i++) {
            for (int j = 0; j < v1.size(); j++) {
                if (v1[j] == v1[i]) {
                    continue;
                }
                int num = graph.numConnections(v1[i], v1[j]);
                if (num) {
                    graph.increasePriority(v1[i], num);
                }
            }
            graph.priorityDone(v1[i]);
        }

        /*remove duplicates from the second vector */
        sort(v2.begin(), v2.end());
        v2.erase(unique(v2.begin(), v2.end()), v2.end());

        /*calculate priorities for second layer*/
        for (int i = 0; i < v2.size(); i++) {
            for (int j = 0; j < v1.size(); j++) {
                if (v2[i] == v1[j]) {
                    continue;
                }
                int num = graph.numConnections(v2[i], v1[j]);
                if (num) {
                    graph.increasePriority(v2[i], num);
                }
            }
        }

        /*remove the duplicates from the vectors, so only 1 per actor*/
        sort(v1.begin(), v1.end());
        v1.erase(unique(v1.begin(), v1.end()), v1.end());

        /*create a PQ for the first set*/
        priority_queue<pair<int, string>, vector<pair<int, string>>, Alpha>
            pq1 = priority_queue<pair<int, string>, vector<pair<int, string>>,
                                 Alpha>();
        priority_queue<pair<int, string>, vector<pair<int, string>>, Alpha>
            pq2 = priority_queue<pair<int, string>, vector<pair<int, string>>,
                                 Alpha>();

        /*populate the pq's with pairs for each node and its priority*/
        for (int i = 0; i < v1.size(); i++) {
            pq1.push(graph.createPair(v1[i]));
        }
        for (int i = 0; i < v2.size(); i++) {
            pq2.push(graph.createPair(v2[i]));
        }

        /*output the top whatever from the pq's into the output files*/
        int pq1size = pq1.size();
        int pq2size = pq2.size();

        for (int i = 0; (i < MAX) && (i < pq1size); i++) {
            string actora = (pq1.top()).second;
            pq1.pop();
            firstOutfile << actora << '\t';
        }
        firstOutfile << endl;

        for (int i = 0; (i < MAX) && (i < pq2size); i++) {
            string actorb = (pq2.top()).second;
            pq2.pop();
            secondOutfile << actorb << '\t';
        }
        secondOutfile << endl;

        /*reset the priorities on the nodes*/
        graph.resetGraph();
    }
}

/*  Main program that runs the compress */
int main(int argc, char* argv[]) {
    char* inFilename = argv[1];
    char* predictions = argv[2];
    char* firstFilename = argv[3];
    char* secondFilename = argv[4];

    /*write the header*/
    ofstream firstoutFile; /*open outFile, write the 256 line header*/
    firstoutFile.open(firstFilename);
    firstoutFile << "Actor1,Actor2,Actor3,Actor4" << endl;

    ofstream secondoutFile; /*open outFile, write the 256 line header*/
    secondoutFile.open(secondFilename);
    secondoutFile << "Actor1,Actor2,Actor3,Actor4" << endl;

    /*PART 2 : READ THE PAIRS, AND FOR EACH ACTOR CALL LINKPREDICTOR*/
    ifstream pfile(predictions);
    bool have_header = false;
    vector<string> record;

    // keep reading lines until the end of file is reached
    while (pfile) {
        string s;
        // get the next line
        if (!getline(pfile, s)) break;
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        istringstream ss(s);
        while (ss) {
            string str;
            // get the next string before hitting a tab character and put it in
            // 'str'
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }
    }

    /*now call LINK PREDICTOR on all the actors*/
    linkpredictor(record, inFilename, firstoutFile, secondoutFile);

    if (!pfile.eof()) {
        cerr << "Failed to read " << predictions << "!\n";
        return false;
    }
    pfile.close();
    firstoutFile.close();
    secondoutFile.close();

    return true;
}
