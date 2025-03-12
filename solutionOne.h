// Graph.h
#ifndef SOLUTIONONE_H
#define SOLUTIONONE_H

#include <bits/stdc++.h>

#include "Graph.h"

using namespace std;

void solutionOne(Graph g, vector<int> *ham1, vector<int> *ham2) {
  srand(time(0));
  int start1 = rand() % g.n;
  int start2 =
      max_element(g.v[start1].begin(), g.v[start1].end()) - g.v[start1].begin();
  vector<bool> visited = vector<bool>(g.n, false);
  visited[start1] = true;
  ham1->push_back(start1);
  visited[start2] = true;
  ham2->push_back(start2);
  for (int i = 2; i < g.n; i++) {
    vector<int> *current = i % 2 == 0 ? ham1 : ham2;
    int lastElement = current->back();
    int nearest = -1;
    for (int j = 0; j < g.n; j++) {
      if (lastElement == j) continue;
      if (!visited[j] &&
          (nearest == -1 || g.v[lastElement][j] < g.v[lastElement][nearest]))
        nearest = j;
    }
    current->push_back(nearest);
    visited[nearest] = true;
  }
  ham1->push_back(ham1->front());
  ham2->push_back(ham2->front());
};

#endif  // SOLUTIONONE_H
