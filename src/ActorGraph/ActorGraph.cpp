/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined in imdb_2019.tsv. Feel free to modify
 * any/all aspects as you wish.
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Actor.hpp"
#include "Movie.hpp"

using namespace std;

/**
 * Constructor of the Actor graph
 */
ActorGraph::ActorGraph(void) {
    actorNodes = new unordered_map<string, Actor*>();
    // movieNodes = new unordered_map<string, Movie*>();
    actormap = new unordered_map<string, vector<string>>();
    moviemap = new unordered_map<string, vector<string>>();
    numActors = 0;
}

/** You can modify this method definition as you wish
 *
 * Load the graph from a tab-delimited file of actor->movie relationships.
 *
 * in_filename - input filename
 * use_weighted_edges - if true, compute edge weights as 1 + (2019 -
 * movie_year), otherwise all edge weights will be 1
 *
 * return true if file was loaded sucessfully, false otherwise
 */
bool ActorGraph::loadFromFile(const char* in_filename,
                              bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);
    // infile.open(in_filename);

    bool have_header = false;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline(infile, s)) break;

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

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor(record[0]);
        string movie_title(record[1]);

        /*create the moviname#@year STRING*/
        string movieYear = record[1] + "#@" + record[2];

        int year = stoi(record[2]);

        /*try to look for the actor and the movie*/
        unordered_map<string, vector<string>>::const_iterator findactor =
            actormap->find(actor);
        unordered_map<string, vector<string>>::const_iterator findmovie =
            moviemap->find(movieYear);
        unordered_map<string, Actor*>::const_iterator actorptr =
            actorNodes->find(actor);

        /*if doesn't exist in our actor map, add to actor map, and add to
         * actor,movie map*/
        Actor* newActor;
        Movie* newMovie;
        bool actorExists = false;
        bool movieExists = true;

        if (findmovie == moviemap->end()) { /*create movie*/
            movieExists = false;
        }

        if (findactor == actormap->end()) {
            newActor = new Actor(actor); /*create actor*/
        } else {
            newActor = actorptr->second; /*get actor*/
            actorExists = true;
        }

        /*NOW YOU CAN ADD TO THE GRAPH*/
        if (!actorExists) {
            actorNodes->insert(make_pair(actor, newActor)); /*add actor*/
            numActors++;
            vector<string>* movies =
                new vector<string>(); /*create movies array*/
            movies->push_back(movieYear);
            actormap->insert(make_pair(actor, *movies));
        } else { /*else, add the MOVIE to that actors movie vector*/
            vector<string> movies = findactor->second; /*get movies array*/
            actormap->erase(findactor);
            movies.push_back(movieYear);
            actormap->insert(make_pair(actor, movies));
        }

        /*if movie doesnt exist, add a pair of movie,actor to the map*/
        if (!movieExists) {
            vector<string>* actors =
                new vector<string>(); /*create actors array*/
            actors->push_back(actor);
            moviemap->insert(make_pair(movieYear, *actors));
        }
        /*else movie exists already, add actor to the movies actor vector*/
        else {
            vector<string> actors = findmovie->second; /*get actors vectr*/
            moviemap->erase(findmovie);
            actors.push_back(actor);
            moviemap->insert(make_pair(movieYear, actors));
        }
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    return true;
}

void ActorGraph::BFS(string actora, string actorb, ostream& outFile) {
    // 1) push the actora's name to the ququeue
    queue<string> q;
    q.push(actora);

    Actor* front; /*to keep track of curr visiting node in traverse*/
    unordered_map<string, vector<string>>::const_iterator findactor, findmovie;
    unordered_map<string, Actor*>::const_iterator actorptr;
    string s, next;
    vector<string> movies, actorsPerMovie;
    bool donezo = false;

    // 3) pop the first actor and find all the films they've done
    while (q.size()) {
        findactor = actormap->find(q.front());
        movies = findactor->second;
        q.pop();

        // 4) for each movie, loop through the in common actors(first layer)
        for (int i = 0; i < movies.size(); i++) {
            // 5) find all the actors in THAT movie
            findmovie = moviemap->find(movies[i]);
            actorsPerMovie = findmovie->second;

            // 6) breadth search and update info for each actor node
            for (int i = 0; i < actorsPerMovie.size(); i++) {
                // make sure you're not repeating nodes
                if (actorsPerMovie[i] == findactor->first) {
                    continue;
                }

                // 7) find that actor in ActorNodes and update prev info
                unordered_map<string, Actor*>::const_iterator actorptr =
                    actorNodes->find(actorsPerMovie[i]);
                Actor* next = actorptr->second;

                if (next->prevActor == "") {
                    next->prevActor = findactor->first;
                    next->prevMovie = findmovie->first;
                }
                if (actorb == actorsPerMovie[i]) {
                    donezo = true;
                    break;
                }
                q.push(actorsPerMovie[i]); /*push this next actor to queue*/
            }
            if (donezo) {
                break;
            }
        }
        if (donezo) {
            break;
        }
    }

    donezo = false; /*reset*/

    /*PART 2  :  WRITE TO THE OUTPUT FILE */
    string nextactor = actorb;         /*start at actorb*/
    vector<pair<string, string>> vect; /* <actor name , movie name> */

    while (1) {
        unordered_map<string, Actor*>::const_iterator actoritr =
            actorNodes->find(nextactor);
        Actor* actorbptr = actoritr->second;

        if (actorbptr->actorName == actora) {
            donezo = true;
            break;
        }
        vect.push_back(make_pair(actorbptr->prevActor, actorbptr->prevMovie));
        nextactor = actorbptr->prevActor; /*go to prev actor*/
    }

    for (int i = 0; i < vect.size(); i++) {
        pair<string, string> pp = vect[vect.size() - 1 - i];
        outFile << "(" << pp.first << ")--[" << pp.second << "]-->";
    }

    /*NOW PRINT OUT THE ACTOR B SHIT*/
    outFile << "(" << actorb << ")" << endl;
}

/*iterors through the actor nodes and reset the prevActor and prevMovie
 * strings*/
void ActorGraph::resetGraph() {
    unordered_map<string, Actor*>::const_iterator itr = actorNodes->begin();
    unordered_map<string, Actor*>::const_iterator en = actorNodes->end();

    while (itr != en) {
        itr->second->prevActor = "";
        itr->second->prevMovie = "";
        ++itr;
    }
}