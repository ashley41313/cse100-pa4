/*
 * movietraveler.cpp
 * Author: Ashley Eckert
 * Date:   December 3, 2019
 *
 * This file creates an actor graph adn finds the shortest path
 * to visit each node in the graph.
 */
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "ActorGraph.hpp"
using namespace std;

/*  Main program that runs the compress */
int main(int argc, char* argv[]) {
    char* inFilename = argv[1];
    char* outFilename = argv[2];

    /*create the graph from the infile*/
    ActorGraph graph;
    graph.loadFromFile(inFilename, 0);

    /*write the header*/
    ofstream outFile; /*open outFile, write the 256 line header*/
    outFile.open(outFilename);
    outFile << "(actor)<--[movie#@year]-->(actor)" << endl;

    /*PART 2 : call KRUSKALS algorithm on the graph*/
    graph.kruskals(outFile);

    /*close the outfile*/
    outFile.close();
    return true;
}
