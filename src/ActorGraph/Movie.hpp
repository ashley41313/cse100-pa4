#ifndef movie_hpp
#define movie_hpp

#include <math.h>
#include <string>
#include <vector>

using namespace std;

/** The data point with given features **/
class Movie {
  public:
    string movieName;
    string longMovieName;
    int year;

    /*default constructor*/
    Movie();

    Movie(string& c, string& d, int e)
        : movieName(c), longMovieName(d), year(e) {}
};
#endif