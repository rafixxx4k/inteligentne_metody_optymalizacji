// Graph.h
#ifndef SOLUTIONTWO_H
#define SOLUTIONTWO_H

#include <bits/stdc++.h>

#include "Graph.h"

using namespace std;

void solutionOne(Graph g, vector<int> *ham1, vector<int> *ham2) {
  srand(time(0));
  int start1 = rand() % g.n;
  int start2 =
      max_element(g.v[start1].begin(), g.v[start1].end()) - g.v[start1].begin();
  vector<bool> visited = vector<bool>(g.n, false);
  // TODO: Implement solution two
};

#endif  // SOLUTIONTWO_H
