#include "greedy.h"

void greedyNearest(Graph g, vector<int> *ham1, vector<int> *ham2) {
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


  void greedyCycle(Graph g, vector<int> *ham1, vector<int> *ham2) {
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
  void random(Graph g, vector<int> *ham1, vector<int> *ham2){
    srand(time(0));
    auto rng = default_random_engine{static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count())};
    vector<int> order(g.n);
    for (int i = 0; i < g.n; i++) {
      order[i] = i;
    }
    shuffle(order.begin(), order.end(), rng);
    for (int i = 0; i < g.n; i+=2) {
      ham1->push_back(order[i]);
      ham2->push_back(order[i+1]);
    }
    ham1->push_back(ham1->front());
    ham2->push_back(ham2->front());
  }