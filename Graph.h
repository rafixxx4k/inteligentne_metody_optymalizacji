// Graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

struct Graph {
    string name;
    int n;
    vector<vector<int>> v;
    vector<pair<int, int>> pos;
};

#endif // GRAPH_H
