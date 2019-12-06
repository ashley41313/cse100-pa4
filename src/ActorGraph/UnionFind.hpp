/*
 * UnionFind.hpp
 * Author: Ashley Eckert
 * Date:   December 3, 2019
 *
 * This file holds the function signatures for the UnionFind class
 */

#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

/*Union Find is a class that uses unordered maps for quick access
 * , and provides the structure for our Up Tree data structure*/
class UnionFind {
  private:
    unordered_map<string, string>* data; /*for final part*/
    unordered_map<string, int> ranks;

  public:
    /* Default Constructor*/
    explicit UnionFind(unordered_map<string, string>& ogdata) {
        data = &ogdata;

        /*iterate over DATA, and for each key set rank to 0*/
        unordered_map<string, string>::const_iterator itr = data->begin();
        unordered_map<string, string>::const_iterator en = data->end();
        while (itr != en) {
            ranks[itr->first] = 0;
            ++itr;
        }
    }

    /*the way to unite two groups*/
    void unite(string actora, string actorb);

    /*finds sentinel of an actor*/
    string find(string actor);

    /*compresses a path so that max length is 0*/
    void path_compression(vector<string> path, string sentinel);
};

#endif  // UNIONFIND_HPP
