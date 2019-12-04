/*
 * ActorGraph.cpp
 * Author: Ashley Eckert
 * Date:   December 3, 2019
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined in imdb_2019.tsv. Feel free to modify
 * any/all aspects as you wish.
 */

#include "ActorGraph.hpp"
#include <string.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Actor.hpp"
#define INFINITY 6666666666  // to find shortest distance

using namespace std;

/**
 * Constructor of the Actor graph
 */
ActorGraph::ActorGraph(void) {
    actorNodes = new unordered_map<string, Actor*>();
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

/*same as BFS except this time we'll be pushing to a priority queue, and
 *it will be a priorirty queue of PAIRS, not strings, the second piece
 *of the pair will be the edge weight, so the movies age, that way we'll have
 *the shortest age when we find the path to the actorb
 *pair will be <edgeweight , actor name> */
void ActorGraph::Dijkstra(string actora, string actorb, ostream& outFile) {
    unordered_map<string, Actor*>::const_iterator actor1;
    unordered_map<string, vector<string>>::const_iterator findactor, findmovie;
    vector<string> movies, actorsPerMovie;
    bool donezo = false;
    pair<int, string> p1;
    int currDistance;
    Actor* next;

    priority_queue<pair<int, string>, vector<pair<int, string>>, Alpha> q =
        priority_queue<pair<int, string>, vector<pair<int, string>>, Alpha>();
    q.push(make_pair(0, actora)); /*push edge 0 with name of actor a*/

    actor1 = actorNodes->find(actora);
    next = actor1->second;
    next->distance = 0;

    while (q.size()) {  // 3) pop the first actor and films they've done
        p1 = q.top();
        q.pop();

        if (p1.second == actorb) {
            break;
        }

        currDistance = p1.first; /*will hold THIS current weight*/
        findactor = actormap->find(p1.second);
        movies = findactor->second;

        actor1 = actorNodes->find(p1.second);
        next = actor1->second;
        next->visited = 1; /*set to has been visited*/

        // 4) for each movie, loop through the in common actors(first layer)
        for (int i = 0; i < movies.size(); i++) {
            // 5) find all the actors in THAT movie
            findmovie = moviemap->find(movies[i]);
            actorsPerMovie = findmovie->second;

            /*get the year of the movie*/
            string r = (movies[i]).substr((movies[i]).size() - 4,
                                          (movies[i]).size() - 1);
            int movieYear = stoi(r);

            // 6) update only if less than distance & not visited
            for (int i = 0; i < actorsPerMovie.size(); i++) {
                if (actorsPerMovie[i] == findactor->first) {
                    continue;
                }
                // 7) find that actor in ActorNodes and update prev info
                actor1 = actorNodes->find(actorsPerMovie[i]);
                next = actor1->second;
                if (next->visited) {
                    continue;
                }
                int newDistance =
                    1 + currDistance + (2019 - movieYear); /*new distance*/

                if (newDistance < next->distance) {
                    next->prevActor = findactor->first;
                    next->prevMovie = findmovie->first;
                    next->distance = newDistance;
                    q.push(make_pair(newDistance, actorsPerMovie[i]));
                }
            }
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
        itr->second->priority = 0;
        itr->second->visited = 0;
        itr->second->done = 0;
        itr->second->distance = INFINITY;
        ++itr;
    }
}

/*used in linkpredictor
 * will be used to only get the first 2 layers of BFS.**/
vector<string> ActorGraph::partialBFS(string actora, int layer) {
    // 1) push the actora's name to the ququeue
    queue<string> q;
    q.push(actora);

    unordered_map<string, vector<string>>::const_iterator findactor, findmovie;
    unordered_map<string, Actor*>::const_iterator actorptr;
    string s, next;
    vector<string> movies, actorsPerMovie;
    vector<string> layerone, layertwo;
    int level = 1;

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
                if ((actorsPerMovie[i] == findactor->first) ||
                    (actorsPerMovie[i] == actora)) {
                    continue;
                }

                // 7) find that actor in ActorNodes and update prev info
                unordered_map<string, Actor*>::const_iterator actorptr =
                    actorNodes->find(actorsPerMovie[i]);
                Actor* next = actorptr->second;

                if (next->visited && (level != 1)) {
                    continue;
                }

                if (level == 1) {
                    q.push(actorsPerMovie[i]); /*we'll only want to do this
                                                  again for the second layer*/
                    next->visited = true;
                }

                /*push this next actor to queue*/
                if (level == 1) {
                    layerone.push_back(actorsPerMovie[i]);
                } else {
                    layertwo.push_back(actorsPerMovie[i]);
                }
            }
        }
        level++;
    }

    if (layer == 1) {
        return layerone;
    } else {
        return layertwo;
    }
}

/*count the number of DIRECT connections between two nodes*/
int ActorGraph::numConnections(string actora, string actorb) {
    int numConnects = 0; /*to count num of connections between the two nodes*/
    unordered_map<string, vector<string>>::const_iterator findactor, findmovie;
    unordered_map<string, Actor*>::const_iterator actorptr =
        actorNodes->find(actora);
    Actor* thisactor = actorptr->second;

    if (thisactor->done) {
        return 0;
    }

    string s, next;
    vector<string> movies, actorsPerMovie;

    findactor = actormap->find(actora);
    movies = findactor->second;

    for (int i = 0; i < movies.size(); i++) { /*loop through all movies*/

        findmovie = moviemap->find(movies[i]); /*actors in that movie*/
        actorsPerMovie = findmovie->second;

        for (int i = 0; i < actorsPerMovie.size(); i++) { /*loop actors in mov*/
            if (actorsPerMovie[i] == findactor->first) {
                continue;
            }
            if (actorb == actorsPerMovie[i]) {
                numConnects++;
            }
        }
    }

    return numConnects;
}

/*will increase the priority for that actornode*/
void ActorGraph::increasePriority(string actor, int num) {
    unordered_map<string, Actor*>::const_iterator actorptr =
        actorNodes->find(actor);
    Actor* next = actorptr->second;
    next->priority += num;
}

/*makes a pair out of the actors name and it's corresponding movie*/
pair<int, string> ActorGraph::createPair(string actor) {
    unordered_map<string, Actor*>::const_iterator actorptr =
        actorNodes->find(actor);
    Actor* next = actorptr->second;

    /* (prioity number, actor name) */
    return (make_pair(next->priority, actor));
}

/*set's the actornodes variable done to 1 so that we know
 * we've worked on it */
void ActorGraph::priorityDone(string actor) {
    unordered_map<string, Actor*>::const_iterator actorptr =
        actorNodes->find(actor);
    Actor* next = actorptr->second;
    next->done = 1;
}