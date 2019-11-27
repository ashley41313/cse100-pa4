/*FILE HEADER : ashley eckert
 * this file is to test the HCTree. It tests methods like insert
 * and build and encode*/
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "ActorGraph.hpp"

using namespace std;
using namespace testing;

/*creates a class to use again in other tests*/
class SimpleGraphFixture : public ::testing::Test {
  protected:
    ActorGraph graph;

  public:
    SimpleGraphFixture() {
        // initialization code here
        ActorGraph graph;
        char name[] = "data/imdb_2019.tsv";
        graph.loadFromFile(name, 0);

        int c;
    }
};

/*tests the encode method*/
TEST_F(SimpleGraphFixture, PARTIAL_BFS) {
    vector<string> layerone = graph.partialBFS("Kevin Bacon", 1);
}
