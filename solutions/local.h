// Graph.h
#ifndef LOCAL_H
#define LOCAL_H

#include <bits/stdc++.h>

#include "../Graph.h"

using namespace std;

void localSteepVert(Graph g, vector<int> *ham1, vector<int> *ham2);
void localGreedyVert(Graph g, vector<int> *ham1, vector<int> *ham2);
void localSteepEdge(Graph g, vector<int> *ham1, vector<int> *ham2);
void localGreedyEdge(Graph g, vector<int> *ham1, vector<int> *ham2);
void localRandom(Graph g, vector<int> *ham1, vector<int> *ham2);

#endif  // LOCAL_H
