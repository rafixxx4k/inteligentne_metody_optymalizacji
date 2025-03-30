// Graph.h
#ifndef GREEDY_H
#define GREEDY_H

#include <bits/stdc++.h>

#include "../Graph.h"

using namespace std;

void greedyNearest(Graph g, vector<int> *ham1, vector<int> *ham2);
void greedyCycle(Graph g, vector<int> *ham1, vector<int> *ham2);
void random(Graph g, vector<int> *ham1, vector<int> *ham2);

#endif  // GREEDY_H
