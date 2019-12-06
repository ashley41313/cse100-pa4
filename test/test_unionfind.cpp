/*FILE HEADER : ashley eckert
 * this file is to test the unionfind class. It tests methods like insert
 * and build*/
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "ActorGraph.hpp"
#include "UnionFind.hpp"

using namespace std;
using namespace testing;
class SimpleGraphFixture : public ::testing::Test {
  protected:
    ActorGraph graph;

  public:
    SimpleGraphFixture() {
        // initialization code here

        int c;
    }
};
/*tests the basic functions of UNIONFIND method*/
TEST_F(SimpleGraphFixture, UNION_FIND) {
    /*test on small graph*/
    cout << "crying" << endl;
    ActorGraph graph;
    graph.loadFromFile("data/imdb_small_sample.tsv", 0);

    graph.createUnion();
    
}
