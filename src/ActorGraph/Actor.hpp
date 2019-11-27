#ifndef actor_hpp
#define actor_hpp

#include <math.h>
#include <string>
#include <vector>

using namespace std;

/** The data point with given features **/
class Actor {
  public:
    string actorName;
    string prevActor;
    string prevMovie;
    int priority;
    bool visited;
    bool done;

    /*default constructor*/
    Actor();

    Actor(string& c) : actorName(c) {
        priority = 0;
        visited = 0;
        done = 0;
    }
};

#endif