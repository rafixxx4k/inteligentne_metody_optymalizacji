#include "smart.h"
int smartCalcDistance(Graph *g, vector<int> *ham1, vector<int> *ham2) {
  ham1->push_back(ham1->front());
  ham2->push_back(ham2->front());
  int cost = 0;
  for (int i = 0; i < ham1->size(); i++) {
    cost += g->v[(*ham1)[i]][(*ham1)[(i + 1) % ham1->size()]];
  }
  for (int i = 0; i < ham2->size(); i++) {
    cost += g->v[(*ham2)[i]][(*ham2)[(i + 1) % ham2->size()]];
  }
  ham1->pop_back();
  ham2->pop_back();
  return cost;
}

int smartSwapCycle(Graph *g, vector<int> *ham1, vector<int> *ham2, int v1,
                   int v2) {
  int cost = 0;
  if ((*ham1)[v1] == (*ham2)[v2]) return 0;
  if ((*ham1)[(v1 + 1) % ham1->size()] == (*ham2)[v2]) {
    cost -= g->v[(*ham1)[v1]][(*ham1)[(v1 - 1 + ham1->size()) % ham1->size()]];
    cost -= g->v[(*ham2)[v2]][(*ham2)[(v2 + 1) % ham2->size()]];
    cost += g->v[(*ham1)[v1]][(*ham2)[(v2 + 1) % ham2->size()]];
    cost += g->v[(*ham1)[(v1 - 1 + ham1->size()) % ham1->size()]][(*ham2)[v2]];
    return cost;
  }
  if ((*ham2)[(v2 + 1) % ham2->size()] == (*ham1)[v1]) {
    cost -= g->v[(*ham2)[v2]][(*ham2)[(v2 - 1 + ham2->size()) % ham2->size()]];
    cost -= g->v[(*ham1)[v1]][(*ham1)[(v1 + 1) % ham1->size()]];
    cost += g->v[(*ham2)[v2]][(*ham1)[(v1 + 1) % ham1->size()]];
    cost += g->v[(*ham2)[(v2 - 1 + ham2->size()) % ham2->size()]][(*ham1)[v1]];
    return cost;
  }
  cost += g->v[(*ham1)[v1]][(*ham2)[(v2 + 1) % ham2->size()]];
  cost += g->v[(*ham1)[(v1 + 1) % ham1->size()]][(*ham2)[v2]];
  cost += g->v[(*ham1)[v1]][(*ham2)[(v2 - 1 + ham2->size()) % ham2->size()]];
  cost += g->v[(*ham1)[(v1 - 1 + ham1->size()) % ham1->size()]][(*ham2)[v2]];

  cost -= g->v[(*ham1)[v1]][(*ham1)[(v1 + 1) % ham1->size()]];
  cost -= g->v[(*ham1)[v1]][(*ham1)[(v1 - 1 + ham1->size()) % ham1->size()]];
  cost -= g->v[(*ham2)[v2]][(*ham2)[(v2 + 1) % ham2->size()]];
  cost -= g->v[(*ham2)[v2]][(*ham2)[(v2 - 1 + ham2->size()) % ham2->size()]];

  return cost;
  // cout<<"oldCost"<<oldCost<<" new cost: "<< cost<<endl;
}

int smartSwapEdges(Graph *g, vector<int> *ham, int v1, int v2) {
  if (v1 == v2) return 0;
  if ((*ham)[(v1 + 1) % ham->size()] == (*ham)[v2] ||
      (*ham)[(v2 + 1) % ham->size()] == (*ham)[v1]) {
    return 1e9;
  }
  int cost =
      g->v[(*ham)[(v1 + 1) % ham->size()]][(*ham)[(v2 + 1) % ham->size()]] +
      g->v[(*ham)[v1]][(*ham)[v2]] -
      g->v[(*ham)[(v1 + 1) % ham->size()]][(*ham)[v1]] -
      g->v[(*ham)[(v2 + 1) % ham->size()]][(*ham)[v2]];
  return cost;
}

struct Move {
  int cost;
  int aminus, a, aplus, bminus, b, bplus;
  Move() {}
  Move(int c, int i, int j, vector<int> *ham1, vector<int> *ham2) {
    if (i >= 100 || j >= 100 || i < 0 || j < 0) cout << i << " " << j << endl;
    cost = c;
    aminus = (*ham1)[(i - 1 + ham1->size()) % ham1->size()];
    a = (*ham1)[i];
    aplus = (*ham1)[(i + 1) % ham1->size()];
    bminus = (*ham2)[(j - 1 + ham2->size()) % ham2->size()];
    b = (*ham2)[j];
    bplus = (*ham2)[(j + 1) % ham2->size()];
  }
};
struct CompareByCost {
  bool operator()(const Move &m1, const Move &m2) const {
    return m1.cost < m2.cost ||
           (m1.cost == m2.cost && (m1.a != m2.a || m1.b != m2.b)) ||
           (m1.cost == m2.cost && m1.a == m2.a && m1.b == m2.b &&
            (m1.aminus != m2.aminus || m1.aplus != m2.aplus ||
             m1.bminus != m2.bminus || m1.bplus != m2.bplus));
    // Add more tie-breakers if needed to avoid duplicates being seen as equal
  }
};

void caching(Graph g, vector<int> *ham1, vector<int> *ham2) {
  srand(time(0));
  ham1->pop_back();
  ham2->pop_back();
  vector<vector<int> *> ham = {ham1, ham2};
  vector<int> positions(g.n, 0);
  vector<int> whichCycle(g.n, 0);

  for (int i = 0; i < ham1->size(); i++) {
    whichCycle[(*ham1)[i]] = 0;
    positions[(*ham1)[i]] = i;
  }
  for (int i = 0; i < ham2->size(); i++) {
    whichCycle[(*ham2)[i]] = 1;
    positions[(*ham2)[i]] = i;
  }
  std::set<Move, CompareByCost> que;
  vector<pair<int, int>> cords = {{-1, -1}, {-1, 0}, {0, -1},
                                  {0, 1},   {1, 0},  {1, 1}};
  vector<pair<vector<int> *, vector<int> *>> pairs = {
      {ham1, ham1}, {ham2, ham2}, {ham1, ham2}};
  for (auto [h1, h2] : pairs) {
    for (int i = 0; i < h1->size(); i++) {
      for (int j = 0; j < h2->size(); j++) {
        int cost, costRback = 0, costRforward = 0;
        if (h1 == h2) {
          cost = smartSwapEdges(&g, h1, i, j);
          costRback =
              smartSwapEdges(&g, h1, i, (j - 1 + h2->size()) % h2->size());
          costRforward =
              smartSwapEdges(&g, h1, (i - 1 + h1->size()) % h1->size(), j);
        } else {
          cost = smartSwapCycle(&g, h1, h2, i, j);
          costRback =
              smartSwapCycle(&g, h1, h2, i, (j - 1 + h2->size()) % h2->size());
          costRforward =
              smartSwapCycle(&g, h1, h2, (i - 1 + h1->size()) % h1->size(), j);
        }

        if (cost < 0) que.insert(Move(cost, i, j, h1, h2));
        if (costRback < 0)
          que.insert(
              Move(costRback, i, (j - 1 + h2->size()) % h2->size(), h1, h2));
        if (costRforward < 0)
          que.insert(
              Move(costRforward, (i - 1 + h1->size()) % h1->size(), j, h1, h2));
      }
    }
  }
  bool warning = false;
  while (!que.empty()) {
    Move m;
    int i, j;
    int skip = 0;
    int numberOfChanges = 1;
    vector<int> *aham, *bham;
    auto it = que.begin();
    while (it != que.end()) {
      m = *it;

      i = positions[m.a];
      j = positions[m.b];
      aham = ham[whichCycle[m.a]];
      bham = ham[whichCycle[m.b]];
      if (aham != bham && smartSwapCycle(&g, aham, bham, i, j) > 0){
        it = que.erase(it);
        continue;
      }
      else if (aham == bham && smartSwapEdges(&g, aham, i, j) > 0){
        it = que.erase(it);
        continue;
      }
      int numberOfChanges = 0;
      numberOfChanges +=
          m.aminus != (*aham)[(i - 1 + aham->size()) % aham->size()];
      numberOfChanges += m.aplus != (*aham)[(i + 1) % aham->size()];
      numberOfChanges +=
          m.bminus != (*bham)[(j - 1 + bham->size()) % bham->size()];
      numberOfChanges += m.bplus != (*bham)[(j + 1) % bham->size()];
      if (numberOfChanges == 0) {
        que.erase(it);
        break;
      } else if (numberOfChanges == 1 || numberOfChanges == 3) {
        it = que.erase(it);
      } else {
        ++it;
      }
      skip++;
    }
    if(it == que.end() && warning) {
      break;
    }
    if (it == que.end()) {
      warning = true;
      for (auto [h1, h2] : pairs) {
        for (int i = 0; i < h1->size(); i++) {
          for (int j = 0; j < h2->size(); j++) {
            int cost;
            if (h1 == h2)
              cost = smartSwapEdges(&g, h1, i, j);
            else
              cost = smartSwapCycle(&g, h1, h2, i, j);
            if (cost < 0) que.insert(Move(cost, i, j, h1, h2));
          }
        }
      }
      continue;
    }
    warning = false;
    if (aham != bham) {
      positions[m.a] = j;
      positions[m.b] = i;
      whichCycle[m.a] = (whichCycle[m.a] + 1) % 2;
      whichCycle[m.b] = (whichCycle[m.b] + 1) % 2;
      swap((*aham)[i], (*bham)[j]);
    } else {
      if (i > j) swap(i, j);
      reverse(aham->begin() + i + 1, aham->begin() + j + 1);
      for (int x = i + 1; x <= j; x++) {
        positions[(*aham)[x]] = x;
      }
    }
    // add to the queue
    for (auto [n, m] : cords) {
      int ni = (i + n + aham->size()) % aham->size();
      int nj = (j + m + bham->size()) % bham->size();
      int cost, costRback = 0, costRforward = 0;
      if (aham == bham) {
        cost = smartSwapEdges(&g, aham, ni, nj);
        costRback = smartSwapEdges(&g, aham, ni,
                                   (nj - 1 + bham->size()) % bham->size());
        costRforward = smartSwapEdges(
            &g, aham, (ni - 1 + aham->size()) % aham->size(), nj);
      } else {
        cost = smartSwapCycle(&g, aham, bham, ni, nj);
        costRback = smartSwapCycle(&g, aham, bham, ni,
                                   (nj - 1 + bham->size()) % bham->size());
        costRforward = smartSwapCycle(
            &g, aham, bham, (ni - 1 + aham->size()) % aham->size(), nj);
      }

      if (cost < 0) {
        // cout << "insert normal" << endl;
        que.insert(Move(cost, ni, nj, aham, bham));
      }
      if (costRback < 0) {
        // cout << "insert back" << endl;
        que.insert(Move(costRback, ni, (nj - 1 + bham->size()) % bham->size(),
                        aham, bham));
      }
      if (costRforward < 0) {
        // cout << "insert forward" << endl;
        que.insert(Move(costRforward, (ni - 1 + aham->size()) % aham->size(),
                        nj, aham, bham));
      }
    }
  }
  ham1->push_back(ham1->front());
  ham2->push_back(ham2->front());
}

void candidate(Graph g, vector<int> *ham1, vector<int> *ham2) {
  srand(time(0));
  ham1->pop_back();
  ham2->pop_back();
  vector<vector<pair<int, int>>> nabor(g.n, vector<pair<int, int>>(g.n));
  vector<int> positions(g.n, 0);
  vector<int> whichCycle(g.n, 0);
  for (int i = 0; i < g.n; i++) {
    for (int j = 0; j < g.n; j++) {
      if (i == j) continue;
      nabor[i][j] = {g.v[i][j], j};
    }
  }
  for (int i = 0; i < ham1->size(); i++) {
    whichCycle[(*ham1)[i]] = 0;
    positions[(*ham1)[i]] = i;
  }
  for (int i = 0; i < ham2->size(); i++) {
    whichCycle[(*ham2)[i]] = 1;
    positions[(*ham2)[i]] = i;
  }
  for (int i = 0; i < g.n; i++) sort(nabor[i].begin(), nabor[i].end());
  // int licznik = 0;
  while (true) {  // until no better solution
    int bestV1, bestV2;
    int bestCost = 0;
    int move = 0;
    int bestCycle;
    vector<vector<int> *> ham = {ham1, ham2};
    for (int i = 0; i < g.n; i++) {
      int cycle = whichCycle[i];
      int a = positions[i];
      for (int j = 0; j < 50; j++) {
        // licznik++;
        int cost;
        int succ = nabor[i][j].second;
        if (succ == i) continue;
        if (whichCycle[succ] == cycle) {
          int b = positions[succ];
          cost = smartSwapEdges(&g, ham[cycle], a, b);
          // cout << "same cycle" << endl;
          // cout << "cost: " << cost << " best: " << bestCost <<" move: "<<
          // move<< endl;
          if (cost < bestCost) {
            move = -1;
            bestCost = cost;
            bestV1 = a;
            bestV2 = b;
            bestCycle = cycle;
          }

        } else {
          // a is in the first cycle and b is in the second cycle
          int A = a;
          int B = find(ham[whichCycle[succ]]->begin(),
                       ham[whichCycle[succ]]->end(), succ) -
                  ham[whichCycle[succ]]->begin();
          if (cycle > whichCycle[succ]) swap(A, B);
          vector<int> costs(4, 0);
          // a-1 b a | b-1 a+1 b+1
          costs[0] =
              smartSwapCycle(&g, ham[0], ham[1], (A + 1) % ham[0]->size(), B);
          swap((*ham[0])[(A + 1) % ham[0]->size()], (*ham[1])[B]);
          costs[0] +=
              smartSwapCycle(&g, ham[0], ham[0], A, (A + 1) % ham[0]->size());
          swap((*ham[0])[(A + 1) % ham[0]->size()], (*ham[1])[B]);
          // // a b a+1 | b-1 a-1 b+1
          // costs[1] = smartSwapCycle(
          //     &g, ham[0], ham[1], (A - 1 + ham[0]->size()) % ham[0]->size(), B);
          // swap((*ham[0])[(A - 1 + ham[0]->size()) % ham[0]->size()],
          //      (*ham[1])[B]);
          // costs[1] += smartSwapCycle(&g, ham[0], ham[0], A,
          //                            (A - 1 + ham[0]->size()) % ham[0]->size());
          // swap((*ham[0])[(A - 1 + ham[0]->size()) % ham[0]->size()],
          //      (*ham[1])[B]);
          // // a-1 b+1 a+1 | b-1 a b
          // costs[2] =
          //     smartSwapCycle(&g, ham[0], ham[1], A, (B + 1) % ham[1]->size());
          // swap((*ham[0])[A], (*ham[1])[(B + 1) % ham[1]->size()]);
          // costs[2] +=
          //     smartSwapCycle(&g, ham[1], ham[1], B, (B + 1) % ham[1]->size());
          // swap((*ham[0])[A], (*ham[1])[(B + 1) % ham[1]->size()]);
          // // a-1 b-1 a+1 | b a b+1
          // costs[3] = smartSwapCycle(&g, ham[0], ham[1], A,
          //                           (B - 1 + ham[1]->size()) % ham[1]->size());
          // swap((*ham[0])[A],
          //      (*ham[1])[(B - 1 + ham[1]->size()) % ham[1]->size()]);
          // costs[3] += smartSwapCycle(&g, ham[1], ham[1], B,
          //                            (B - 1 + ham[1]->size()) % ham[1]->size());
          // swap((*ham[0])[A],
          //      (*ham[1])[(B - 1 + ham[1]->size()) % ham[1]->size()]);
          int costPos = min_element(costs.begin(), costs.end()) - costs.begin();
          int cost = costs[costPos];

          if (cost < bestCost) {
            move = costPos + 1;
            bestCost = cost;
            bestV1 = A;
            bestV2 = B;
          }
        }
        // cout<<licznik<<endl;
      }
    }
    if (move == 0) {
      break;
    }
    if (move == -1) {
      if (bestV1 > bestV2) swap(bestV1, bestV2);
      reverse(ham[bestCycle]->begin() + bestV1 + 1,
              ham[bestCycle]->begin() + bestV2 + 1);
      for (int i = bestV1 + 1; i <= bestV2; i++) {
        positions[(*ham[bestCycle])[i]] = i;
      }
    }
    if (move == 1) {
      int a = (*ham[0])[bestV1];
      int aplus = (*ham[0])[(bestV1 + 1) % ham[0]->size()];
      int b = (*ham[1])[bestV2];
      positions[a] = (bestV1 + 1) % ham[0]->size();
      positions[aplus] = bestV2;
      positions[b] = bestV1;
      whichCycle[(*ham[0])[(bestV1 + 1) % ham[0]->size()]] = 1;
      whichCycle[(*ham[1])[bestV2]] = 0;
      swap((*ham[0])[(bestV1 + 1) % ham[0]->size()], (*ham[1])[bestV2]);
      swap((*ham[0])[bestV1], (*ham[0])[(bestV1 + 1) % ham[0]->size()]);
    }
    // if (move == 2) {
    //   int a = (*ham[0])[bestV1];
    //   int aminus = (*ham[0])[(bestV1 - 1 + ham[0]->size()) % ham[0]->size()];
    //   int b = (*ham[1])[bestV2];
    //   positions[a] = (bestV1 - 1 + ham[0]->size()) % ham[0]->size();
    //   positions[aminus] = bestV2;
    //   positions[b] = bestV1;
    //   whichCycle[(*ham[0])[(bestV1 - 1 + ham[0]->size()) % (*ham[0]).size()]] =
    //       1;
    //   whichCycle[(*ham[1])[bestV2]] = 0;
    //   swap((*ham[0])[(bestV1 - 1 + ham[0]->size()) % ham[0]->size()],
    //        (*ham[1])[bestV2]);
    //   swap((*ham[0])[bestV1],
    //        (*ham[0])[(bestV1 - 1 + ham[0]->size()) % ham[0]->size()]);
    // }
    // if (move == 3) {
    //   int a = (*ham[0])[bestV1];
    //   int bplus = (*ham[1])[(bestV2 + 1) % ham[1]->size()];
    //   int b = (*ham[1])[bestV2];
    //   positions[a] = bestV2;
    //   positions[bplus] = bestV1;
    //   positions[b] = (bestV2 + 1) % ham[1]->size();
    //   whichCycle[(*ham[0])[bestV1]] = 1;
    //   whichCycle[(*ham[1])[(bestV2 + 1) % (*ham[1]).size()]] = 0;
    //   swap((*ham[0])[bestV1], (*ham[1])[(bestV2 + 1) % ham[1]->size()]);
    //   swap((*ham[1])[bestV2], (*ham[1])[(bestV2 + 1) % ham[1]->size()]);
    // }
    // if (move == 4) {
    //   int a = (*ham[0])[bestV1];
    //   int bminus = (*ham[1])[(bestV2 - 1 + ham[1]->size()) % ham[1]->size()];
    //   int b = (*ham[1])[bestV2];
    //   positions[a] = bestV2;
    //   positions[bminus] = bestV1;
    //   positions[b] = (bestV2 - 1 + ham[1]->size()) % ham[1]->size();
    //   whichCycle[(*ham[0])[bestV1]] = 1;
    //   whichCycle[(
    //       *ham[1])[(bestV2 - 1 + (*ham[1]).size()) % (*ham[1]).size()]] = 0;
    //   swap((*ham[0])[bestV1],
    //        (*ham[1])[(bestV2 - 1 + ham[1]->size()) % ham[1]->size()]);
    //   swap((*ham[1])[bestV2],
    //        (*ham[1])[(bestV2 - 1 + ham[1]->size()) % ham[1]->size()]);
    // }
    // cout << "end of search" << endl;
    // cout << "cost: " << smartCalcDistance(&g, ham1, ham2) << endl;
    // int sumWhichCycle = accumulate(whichCycle.begin(), whichCycle.end(), 0);
    // cout << "Sum of whichCycle: " << sumWhichCycle << endl;
  }

  ham1->push_back(ham1->front());
  ham2->push_back(ham2->front());
}