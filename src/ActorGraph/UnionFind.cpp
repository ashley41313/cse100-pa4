/*
 * UnionFind.cpp
 * Author: Ashley Eckert
 * Date:   December 3, 2019
 *
 * This file is meant to hold to UnionFind class which is to be used
 * in Kruskals algorithm. It has a union function, a find functon,
 * and path compression.
 */

#include "UnionFind.hpp"

/*unites two paths*/
void UnionFind::unite(string actora, string actorb) {
    string sentinel_a = find(actora);
    string sentinel_b = find(actorb);

    if (sentinel_a != sentinel_b) {
        if (ranks[sentinel_a] < ranks[sentinel_b]) {
            (*data)[sentinel_a] = sentinel_b;
        } else if (ranks[sentinel_a] > ranks[sentinel_b]) {
            (*data)[sentinel_b] = sentinel_a;
        } else {
            (*data)[sentinel_a] = sentinel_b;
            ranks[sentinel_b] += 1;
        }
    }
}

/*finds the sentinel node of an actors name*/
string UnionFind::find(string actor) {
    vector<string> path;

    while ((*data)[actor] != "") {
        path.push_back(actor);
        actor = (*data)[actor];
    }

    path_compression(path, actor);
    return actor;
}

/*makes all paths point to the sentinel actors*/
void UnionFind::path_compression(vector<string> path, string sentinel) {
    for (string x : path) {
        (*data)[x] = sentinel;
    }
}
