/*
 * Actor.hpp
 * Author: Ashley Eckert
 * Date:   December 3, 2019
 *
 * This file holds the Actor class. Which holds info for each actor
 * that are relative to whichever search we end up doing.
 */

#ifndef actor_hpp
#define actor_hpp

#include <limits.h>  //to set threshold to inifinity
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#define INFINITY 6666666666

using namespace std;

/** The data point with given features **/
/*class header:
 * the actor class holds the name of the actor most importantly
 * and the things like the previous edge which is prev actor/movie,
 * and the distance variable*/
class Actor {
  public:
    string actorName;
    string prevActor;
    string prevMovie;
    int priority;
    double distance; /*for dijkstra*/
    bool visited;    /*for BFS to know if we've been here*/
    bool done;

    /*default constructor*/
    Actor();

    Actor(string& c) : actorName(c) {
        distance = INFINITY;
        priority = 0;
        visited = 0;
        done = 0;
    }
};

#endif