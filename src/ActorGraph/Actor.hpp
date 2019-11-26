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
    bool visited;
    string prevActor;
    string prevMovie;

    /*default constructor*/
    Actor();

    Actor(string& c) : actorName(c) { visited = 0; }
};

#endif