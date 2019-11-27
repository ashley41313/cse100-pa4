/*
 * ActorGraph.hpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined imdb_2019.tsv. Feel free to modify
 * any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "Actor.hpp"
#include "Movie.hpp"
// Maybe include some data structures here

/*this is the class and operator that will be used to sort our
 *priority queue first by the # then by the alphabet*/
class Alpha {
  public:
    bool operator()(pair<int, string> const& one,
                    pair<int, string> const& two) {
        return !(one.first > two.first ||
                 ((one.first == two.first) && (one.second < two.second)));
    }
};

using namespace std;

/**
 * TODO: add class header
 */
class ActorGraph {
  protected:
    // Maybe add class data structure(s) here
    unordered_map<string, Actor*>* actorNodes;
    unordered_map<string, vector<string>>* moviemap;
    unordered_map<string, vector<string>>* actormap;
    int numActors;

  public:
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void);

    // Maybe add some more methods here

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
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    void BFS(string actora, string actorb, ostream& outFile);
    void resetGraph();
    vector<string> partialBFS(string actora, int layer);
    int numConnections(string actora, string actorb);
    void increasePriority(string actor, int num);
    pair<int, string> createPair(string actor);
    void priorityDone(string actor);
};

#endif  // ACTORGRAPH_HPP
