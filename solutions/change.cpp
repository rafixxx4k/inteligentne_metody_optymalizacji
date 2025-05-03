#include "change.h"

void MSLS(Graph g, vector<int> *ham1, vector<int> *ham2) {
  cout << "0 ";
  vector<int> best1, best2;
  int bestCost = 1e9;
  srand(time(0));
  for (int i = 0; i < 200; i++) {
    random(g, ham1, ham2);
    caching(g, ham1, ham2);
    int cost = smartCalcDistance(&g, ham1, ham2);

    if (cost < bestCost) {
      // cout<<"found better solution: "<<cost<<endl;
      bestCost = cost;
      best1 = *ham1;
      best2 = *ham2;
    }
    ham1->clear();
    ham2->clear();
  }
  *ham1 = best1;
  *ham2 = best2;
};

void ILS(Graph g, vector<int> *ham1, vector<int> *ham2) {
  int iteration = 0;
  int maxTime = 10310;
  auto start = chrono::high_resolution_clock::now();
  //   cout<<"in ILS"<<endl;
  srand(time(0));
  vector<int> best1, best2;
  random(g, ham1, ham2);
  caching(g, ham1, ham2);
  int currCost = smartCalcDistance(&g, ham1, ham2);
  best1 = *ham1;
  best2 = *ham2;
  //   cout<<"arter initialization"<<endl;
  while (true) {
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    if (duration.count() > maxTime) break;
    ham1->pop_back();
    ham2->pop_back();
    for (int i = 0; i < 16; i++) {
      int inner = rand() % 3, index1 = (rand() % (ham1->size() - 1)) + 1,
          index2 = (rand() % (ham1->size() - 1)) + 1;
      if (inner == 0) swap((*ham1)[index1], (*ham2)[index2]);

      if (index1 > index2) swap(index1, index2);
      if (index1 == index2) continue;
      if (inner == 1) reverse(ham2->begin() + index1, ham2->begin() + index2);
      if (inner == 2) reverse(ham1->begin() + index1, ham1->begin() + index2);
    }
    ham1->push_back(ham1->front());
    ham2->push_back(ham2->front());
    caching(g, ham1, ham2);
    int cost = smartCalcDistance(&g, ham1, ham2);
    if (cost < currCost) {
      //   cout << "iteration: " << iteration << endl;
      //   cout << "found better solution: " << cost << endl;

      currCost = cost;
      best1 = *ham1;
      best2 = *ham2;
    } else {
      *ham1 = best1;
      *ham2 = best2;
    }
    iteration++;
  }
  cout << iteration << " ";
  *ham1 = best1;
  *ham2 = best2;
}
void repair(Graph *g, vector<int> *ham1, vector<int> *ham2) {
  double alpha = 0.6;
  vector<bool> visited(g->n, false);
  for (int i : *ham1) visited[i] = true;
  for (int i : *ham2) visited[i] = true;

  int seft = 0;
  for (bool v : visited)
    if (!v) seft++;

  for (int i = 0; i < seft; i++) {
    int bestVertex = -1, bestPlace = -1;
    double bestRegret = 1e9;
    bool addToFirst = true;

    for (int v = 0; v < g->n; v++) {
      if (visited[v]) continue;

      // Try ham1
      int bestCost1 = INT_MAX, secondBest1 = INT_MAX, bestPos1 = -1;
      for (int j = 1; j < ham1->size(); j++) {
        int a = (*ham1)[j - 1], b = (*ham1)[j];
        int cost = g->v[a][v] + g->v[v][b] - g->v[a][b];
        if (cost < bestCost1) {
          secondBest1 = bestCost1;
          bestCost1 = cost;
          bestPos1 = j;
        } else if (cost < secondBest1) {
          secondBest1 = cost;
        }
      }

      double regret1 = bestCost1;
      // alpha * bestCost1 + (1 - alpha) * (secondBest1 - bestCost1);

      // Try ham2
      int bestCost2 = INT_MAX, secondBest2 = INT_MAX, bestPos2 = -1;
      for (int j = 1; j < ham2->size(); j++) {
        int a = (*ham2)[j - 1], b = (*ham2)[j];
        int cost = g->v[a][v] + g->v[v][b] - g->v[a][b];
        if (cost < bestCost2) {
          secondBest2 = bestCost2;
          bestCost2 = cost;
          bestPos2 = j;
        } else if (cost < secondBest2) {
          secondBest2 = cost;
        }
      }

      double regret2 = bestCost2;
      // alpha * bestCost2 + (1 - alpha) * (secondBest2 - bestCost2);

      if (ham1->size() < 101 && regret1 < bestRegret) {
        bestRegret = regret1;
        bestVertex = v;
        bestPlace = bestPos1;
        addToFirst = true;
      }

      if (ham2->size() < 101 && regret2 < bestRegret) {
        bestRegret = regret2;
        bestVertex = v;
        bestPlace = bestPos2;
        addToFirst = false;
      }
    }

    // Insert the best vertex in the best place
    if (addToFirst) {
      ham1->insert(ham1->begin() + bestPlace, bestVertex);
    } else {
      ham2->insert(ham2->begin() + bestPlace, bestVertex);
    }

    visited[bestVertex] = true;
  }
}
void destroy(Graph *g, vector<int> *ham1, vector<int> *ham2) {
  ham1->pop_back();
  ham2->pop_back();
  int toDestroy = 60;
  int destroyed = 0;
  vector<vector<int> *> ham = {ham1, ham2};
  while (destroyed < toDestroy) {
    int cycle = rand() % 2;
    int start = rand() % (ham[cycle]->size() - 1);
    int len = rand() % min(int(ham[cycle]->size()) - start + 1, 30);
    ham[cycle]->erase(ham[cycle]->begin() + start,
                      ham[cycle]->begin() + start + len);
    destroyed += len;
  }
  ham1->push_back(ham1->front());
  ham2->push_back(ham2->front());
}
void LNS(Graph g, vector<int> *ham1, vector<int> *ham2) {
  srand(time(0));
  int iteration = 0;
  int maxTime = 10310;
  auto start = chrono::high_resolution_clock::now();
  //   cout<<"in ILS"<<endl;
  srand(time(0));
  vector<int> best1, best2;
  random(g, ham1, ham2);
  caching(g, ham1, ham2);
  int currCost = smartCalcDistance(&g, ham1, ham2);
  // cout << "INIT COST: " << currCost << endl;
  // cout << "INIT: " << ham1->size() << " " << ham2->size() << endl;
  best1 = *ham1;
  best2 = *ham2;
  while (true) {
    iteration++;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    if (duration.count() > maxTime) break;

    destroy(&g, ham1, ham2);
    // cout << "Destroyed: " << ham1->size() + ham2->size() << endl;
    repair(&g, ham1, ham2);
    // cout<<"Repair: "<<ham1->size() + ham2->size()<<endl;
    caching(g, ham1, ham2);
    int cost = smartCalcDistance(&g, ham1, ham2);
    // cout<<"Cost: "<<cost<<endl;
    if (cost < currCost) {
      // cout << "iteration: " << iteration << endl;
      // cout << "found better solution: " << cost << endl;
      // cout << "size: " << ham1->size() << " " << ham2->size() << endl;

      currCost = cost;
      best1 = *ham1;
      best2 = *ham2;
    } else {
      *ham1 = best1;
      *ham2 = best2;
    }
    // if (iteration == 10) break;
  }
  cout << iteration << " ";
};
