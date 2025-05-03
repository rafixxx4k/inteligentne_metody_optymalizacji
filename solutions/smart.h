// Graph.h
#ifndef SMART_H
#define SMART_H

#include <bits/stdc++.h>

#include "../Graph.h"

using namespace std;

void caching(Graph g, vector<int> *ham1, vector<int> *ham2);
void candidate(Graph g, vector<int> *ham1, vector<int> *ham2);
int smartCalcDistance(Graph *g, vector<int> *ham1, vector<int> *ham2); 

#endif  // SMART_H
