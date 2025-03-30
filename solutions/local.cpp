#include "greedy.h"

int swapCycle(Graph g, vector<int> *ham1, vector<int> *ham2, int v1, int v2) {
  int cost = 0;
  int cords[] = {-1, 1};
  int vert[] = {v1, v2};
  vector<vector<int> *> ham = {ham1, ham2};
  for (int i : cords)
    for (int j = 0; j < 2; j++)
      for (int k = 0; k < 2; k++) {
        if ((*ham[j])[(vert[j] + i + ham[j]->size()) % ham[j]->size()] ==
            (*ham[(j + 1) % 2])[vert[(j + 1) % 2]]) {
          // cout<<"continue"<<endl;
          continue;
        } else if (j == k) {
          cost -=
              g.v[(*ham[j])[(vert[j] + i + ham[j]->size()) % ham[j]->size()]]
                 [(*ham[k])[vert[k]]];
        } else {
          cost +=
              g.v[(*ham[j])[(vert[j] + i + ham[j]->size()) % ham[j]->size()]]
                 [(*ham[k])[vert[k]]];
        }
      }

  return cost;
}

int swapEdges(Graph g, vector<int> *ham, int v1, int v2) {
  if (v1 == v2) return 0;
  if ((*ham)[(v1 + 1) % ham->size()] == (*ham)[v2] ||
      (*ham)[(v2 + 1) % ham->size()] == (*ham)[v1]) {
    return 1e9;
  }
  int cost =
      g.v[(*ham)[(v1 + 1) % ham->size()]][(*ham)[(v2 + 1) % ham->size()]] +
      g.v[(*ham)[v1]][(*ham)[v2]] -
      g.v[(*ham)[(v1 + 1) % ham->size()]][(*ham)[v1]] -
      g.v[(*ham)[(v2 + 1) % ham->size()]][(*ham)[v2]];
  return cost;
}

void localSteepVert(Graph g, vector<int> *ham1, vector<int> *ham2) {
  // int gen = 1;
  srand(time(0));
  ham1->pop_back();
  ham2->pop_back();
  //   swapCycle(g, ham1, ham1, 1, 2);
  while (true) {  // until no better solution
    int bestV1, bestV2;
    int bestCost = 0;
    pair<vector<int> *, vector<int> *> bestPair;
    vector<pair<vector<int> *, vector<int> *>> pairs = {
        {ham1, ham1}, {ham2, ham2}, {ham1, ham2}};
    for (auto [h1, h2] : pairs) {
      for (int i = 0; i < h1->size(); i++) {
        for (int j = 0; j < h2->size(); j++) {
          int cost = swapCycle(g, h1, h2, i, j);
          if (cost < bestCost) {
            bestPair = {h1, h2};
            bestCost = cost;
            bestV1 = i;
            bestV2 = j;
          }
        }
      }
    }
    if (bestCost >= 0) break;
    swap((*bestPair.first)[bestV1], (*bestPair.second)[bestV2]);
  }
  ham1->push_back(ham1->front());
  ham2->push_back(ham2->front());
};

void localGreedyVert(Graph g, vector<int> *ham1, vector<int> *ham2) {
  srand(time(0));
  auto rng = default_random_engine{};
  vector<int> order(g.n / 2);
  for (int i = 0; i < g.n / 2; i++) {
    order[i] = i;
  }
  ham1->pop_back();
  ham2->pop_back();
  while (true) {  // until no better solution
    bool madeSwap = false;
    vector<pair<vector<int> *, vector<int> *>> pairs = {
        {ham1, ham1}, {ham2, ham2}, {ham1, ham2}};
    shuffle(pairs.begin(), pairs.end(), rng);
    shuffle(order.begin(), order.end(), rng);
    for (auto [h1, h2] : pairs) {
      for (int i = 0; i < h1->size(); i++) {
        for (int j = 0; j < h2->size(); j++) {
          int cost = swapCycle(g, h1, h2, order[i], order[j]);
          if (cost < 0) {
            swap((*h1)[order[i]], (*h2)[order[j]]);
            madeSwap = true;
          }
          if (madeSwap) break;
        }
        if (madeSwap) break;
      }
      if (madeSwap) break;
    }
    if (!madeSwap) break;
  }
  ham1->push_back(ham1->front());
  ham2->push_back(ham2->front());
};

void localSteepEdge(Graph g, vector<int> *ham1, vector<int> *ham2) {
  // int gen = 1;
  srand(time(0));
  ham1->pop_back();
  ham2->pop_back();
  //   swapCycle(g, ham1, ham1, 1, 2);
  while (true) {  // until no better solution
    int bestV1, bestV2;
    int bestCost = 0;
    pair<vector<int> *, vector<int> *> bestPair;
    vector<pair<vector<int> *, vector<int> *>> pairs = {
        {ham1, ham1}, {ham2, ham2}, {ham1, ham2}};
    for (auto [h1, h2] : pairs) {
      for (int i = 0; i < h1->size(); i++) {
        for (int j = 0; j < h2->size(); j++) {
          int cost;
          if (h1 == h2)
            cost = swapEdges(g, h1, i, j);
          else
            cost = swapCycle(g, h1, h2, i, j);
          if (cost < bestCost) {
            bestPair = {h1, h2};
            bestCost = cost;
            bestV1 = i;
            bestV2 = j;
          }
        }
      }
    }
    if (bestCost >= 0) break;
    if (bestPair.first == bestPair.second) {
      if (bestV1 > bestV2) swap(bestV1, bestV2);
      reverse(bestPair.first->begin() + bestV1 + 1, bestPair.first->begin() + bestV2 + 1);
    } else {
      swap((*bestPair.first)[bestV1], (*bestPair.second)[bestV2]);
    }
  }
  ham1->push_back(ham1->front());
  ham2->push_back(ham2->front());
};

void localGreedyEdge(Graph g, vector<int> *ham1, vector<int> *ham2){
  srand(time(0));
  auto rng = default_random_engine{};
  vector<int> order(g.n / 2);
  for (int i = 0; i < g.n / 2; i++) {
    order[i] = i;
  }
  ham1->pop_back();
  ham2->pop_back();
  while (true) {  // until no better solution
    bool madeSwap = false;
    vector<pair<vector<int> *, vector<int> *>> pairs = {
        {ham1, ham1}, {ham2, ham2}, {ham1, ham2}};
    shuffle(pairs.begin(), pairs.end(), rng);
    shuffle(order.begin(), order.end(), rng);
    for (auto [h1, h2] : pairs) {
      for (int i = 0; i < h1->size(); i++) {
        for (int j = 0; j < h2->size(); j++) {
          if (h1 == h2){
            int cost = swapEdges(g, h1, order[i], order[j]);
            if (cost < 0) {
              if(order[i] > order[j]) swap(order[i], order[j]);
              reverse(h1->begin() + order[i] + 1, h1->begin() + order[j] + 1);
              madeSwap = true;
            }
          }
          else{
            int cost = swapCycle(g, h1, h2, order[i], order[j]);
            if (cost < 0) {
              swap((*h1)[order[i]], (*h2)[order[j]]);
              madeSwap = true;
            }
          }
          if (madeSwap) break;
        }
        if (madeSwap) break;
      }
      if (madeSwap) break;
    }
    if (!madeSwap) break;
  }
  ham1->push_back(ham1->front());
  ham2->push_back(ham2->front());
};