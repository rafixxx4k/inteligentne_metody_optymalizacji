// Graph.h
#ifndef SOLUTIONTWO_H
#define SOLUTIONTWO_H

#include <bits/stdc++.h>

#include "Graph.h"

using namespace std;

void solutionTwo(Graph g, vector<int> *ham1, vector<int> *ham2) {
  srand(time(0));
  int start1 = rand() % g.n;
  int start2 =
      max_element(g.v[start1].begin(), g.v[start1].end()) - g.v[start1].begin();
  vector<bool> visited = vector<bool>(g.n, false);
  visited[start1] = true;
  ham1->push_back(start1);
  ham1->push_back(start1);
  visited[start2] = true;
  ham2->push_back(start2);
  ham2->push_back(start2);
  for (int i = 2; i < g.n; i++) {  // for all the vertices
    vector<int> *current = i % 2 == 0 ? ham1 : ham2;
    int lastElement = current->back();
    int bestVertex;
    int bestPlace;
    int bestCost = -1;
    for (int j = 1; j < current->size(); j++) {  // for all places inbetween
      for (int k = 0; k < g.n; k++) {            // try all not visited vertices
        if (visited[k]) continue;
        int addedCost = g.v[(*current)[j - 1]][k] + g.v[k][(*current)[j]] -
                        g.v[(*current)[j - 1]][(*current)[j]];
        if (bestCost == -1 || addedCost < bestCost) {
          bestCost = addedCost;
          bestVertex = k;
          bestPlace = j;
        }
      }
    }

    current->insert(current->begin() + bestPlace, bestVertex);
    visited[bestVertex] = true;
  }
};

#endif  // SOLUTIONTWO_H
