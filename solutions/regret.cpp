#include "greedy.h"

void regret2(Graph g, vector<int> *ham1, vector<int> *ham2) {
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
    int bestVertex, bestPlace, bestRegret;
    bool firstVertex = true;
    for (int k = 0; k < g.n; k++) {  // try all not visited vertices
      if (visited[k]) continue;
      vector<tuple<int, int, int>> regrets;
      regrets.reserve(current->size());
      for (int j = 1; j < current->size(); j++) {  // for all places inbetween
        int addedCost = g.v[(*current)[j - 1]][k] + g.v[k][(*current)[j]] -
                        g.v[(*current)[j - 1]][(*current)[j]];
        regrets.push_back({addedCost, j, k});
      }
      sort(regrets.begin(), regrets.end());
      int regret;
      //   cout<<"iteration: "<<i<<" vertex: "<< k<<" size:
      //   "<<regrets.size()<<endl;
      if (regrets.size() == 1)
        regret = -get<0>(regrets[0]);
      else
        regret = get<0>(regrets[1]) - get<0>(regrets[0]);
      if (firstVertex || regret > bestRegret) {
        firstVertex = false;
        bestRegret = regret;
        bestVertex = get<2>(regrets[0]);
        bestPlace = get<1>(regrets[0]);
      }
    }
    current->insert(current->begin() + bestPlace, bestVertex);
    visited[bestVertex] = true;
  }
};

void regretWeighted(Graph g, vector<int> *ham1, vector<int> *ham2) {
  double alpha = 0.6;
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
    int bestVertex, bestPlace;
    float bestRegret;
    bool firstVertex = true;
    for (int k = 0; k < g.n; k++) {  // try all not visited vertices
      if (visited[k]) continue;
      vector<tuple<int, int, int>> regrets;
      regrets.reserve(current->size());
      for (int j = 1; j < current->size(); j++) {  // for all places inbetween
        int addedCost = g.v[(*current)[j - 1]][k] + g.v[k][(*current)[j]] -
                        g.v[(*current)[j - 1]][(*current)[j]];
        regrets.push_back({addedCost, j, k});
      }
      sort(regrets.begin(), regrets.end());
      double regret;
      //   cout<<"iteration: "<<i<<" vertex: "<< k<<" size:
      //   "<<regrets.size()<<endl;
      if (regrets.size() == 1)
        regret = -get<0>(regrets[0]);
      else
        regret = alpha * double(get<0>(regrets[1]) - get<0>(regrets[0])) -
                 (1 - alpha) * get<0>(regrets[0]);
      if (firstVertex || regret > bestRegret) {
        firstVertex = false;
        bestRegret = regret;
        bestVertex = get<2>(regrets[0]);
        bestPlace = get<1>(regrets[0]);
      }
    }
    current->insert(current->begin() + bestPlace, bestVertex);
    visited[bestVertex] = true;
  }
};