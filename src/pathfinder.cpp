#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "ActorGraph.hpp"
using namespace std;

/*  Main program that runs the compress */
int main(int argc, char* argv[]) {
    const int NUM_ARG = 5; /*4 + executable*/

    char* inFilename = argv[1];
    char* edges = argv[2];
    char* pairsFilename = argv[3];
    char* outFilename = argv[4];

    /*create the graph from the infile*/
    ActorGraph graph;
    graph.loadFromFile(inFilename, 0);

    /*write the header*/
    ofstream outFile; /*open outFile, write the 256 line header*/
    outFile.open(outFilename);
    outFile << "(actor)--[movie#@year]-->(actor)--..." << endl;

    /*PART 2 : READ THE PAIRS, AND FOR EACH PAIR CALL BFS*/
    ifstream pfile(pairsFilename);
    bool have_header = false;
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
        vector<string> record;
        while (ss) {
            string str;
            // get the next string before hitting a tab character and put it in
            // 'str'
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }
        if (record.size() != 2) {
            // we should have exactly 3 columns
            continue;
        }

        /*now call BFS on that pair*/
        graph.BFS(record[0], record[1], outFile);
        graph.resetGraph();
    }

    if (!pfile.eof()) {
        cerr << "Failed to read " << pairsFilename << "!\n";
        return false;
    }
    pfile.close();
    outFile.close();

    return true;
}
