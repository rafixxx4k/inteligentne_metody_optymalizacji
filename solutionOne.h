// Graph.h
#ifndef SOLUTIONONE_H
#define SOLUTIONONE_H

#include <bits/stdc++.h>

#include "Graph.h"

using namespace std;

void greedy(Graph g, vector<int> *ham1, vector<int> *ham2) {
    srand(time(0));
    int start1 = rand() % g.n;
    cout<<start1<<", point x: "<< g.pos[start1].first<<" y: "<< g.pos[start1].second<<endl;
    int start2 = max_element(g.v[start1].begin(), g.v[start1].end()) - g.v[start1].begin();
    cout<<start2<<", point x: "<< g.pos[start2].first<<" y: "<< g.pos[start2].second<<endl;
};

#endif  // SOLUTIONONE_H
