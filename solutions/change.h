// Graph.h
#ifndef CHANGE_H
#define CHANGE_H

#include <bits/stdc++.h>
#include "greedy.h"
#include "local.h"
#include "smart.h"

#include "../Graph.h"

using namespace std;

void MSLS(Graph g, vector<int> *ham1, vector<int> *ham2);
void ILS(Graph g, vector<int> *ham1, vector<int> *ham2);
void LNS(Graph g, vector<int> *ham1, vector<int> *ham2);

#endif  // CHANGE_H