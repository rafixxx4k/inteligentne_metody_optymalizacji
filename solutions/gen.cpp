#include "gen.h"

void gen(Graph g, vector<int> *ham1, vector<int> *ham2) {
  srand(time(0));
  const int POP_SIZE = 20;
  const int MAX_ITER = 10000;
  int bestCost = 2e9;
  vector<vector<int>> population1, population2;
  vector<int> fitness;
  int iteration = 0;
  int maxTime = 10310;
  auto start = chrono::high_resolution_clock::now();

  // === Generate initial population ===
  while (population1.size() < POP_SIZE) {
    vector<int> h1, h2;
    random(g, &h1, &h2);
    caching(g, &h1, &h2);
    int cost = smartCalcDistance(&g, &h1, &h2);

    // Check uniqueness
    bool unique = true;
    for (int f : fitness) {
      if (f == cost) {
        unique = false;
        break;
      }
    }
    if (unique) {
      population1.push_back(h1);
      population2.push_back(h2);
      fitness.push_back(cost);
    }
    if (cost < bestCost) {
      bestCost = cost;
      *ham1 = h1;
      *ham2 = h2;
    }
  }
  while (true) {
    iteration++;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    if (duration.count() > maxTime) break;
    // Select two random distinct parents
    int idx1 = rand() % POP_SIZE;
    int idx2;
    do {
      idx2 = rand() % POP_SIZE;
    } while (idx1 == idx2);

    vector<int> &p1_h1 = population1[idx1];
    vector<int> &p1_h2 = population2[idx1];
    vector<int> &p2_h1 = population1[idx2];
    vector<int> &p2_h2 = population2[idx2];

    // Convert parent tours to edge sets
    auto toEdges = [](const vector<int> &one, const vector<int> &two) {
      set<pair<int, int>> edges;
      for (size_t i = 0; i < one.size() - 1; ++i) {
        int a = one[i];
        int b = one[i + 1];
        if (a > b) swap(a, b);
        edges.insert({a, b});
      }
      for (size_t i = 0; i < two.size() - 1; ++i) {
        int a = two[i];
        int b = two[i + 1];
        if (a > b) swap(a, b);
        edges.insert({a, b});
      }
      return edges;
    };

    auto parent1 = toEdges(p1_h1, p1_h2);
    auto parent2 = toEdges(p2_h1, p2_h2);
    set<pair<int, int>> commonEdges;
    set_intersection(parent1.begin(), parent1.end(), parent2.begin(),
                     parent2.end(), inserter(commonEdges, commonEdges.begin()));

    // Create offspring from p1_h1 by keeping only common edges
    vector<int> offspring1, offspring2;
    for (int i = 0; i < p1_h1.size() - 1; i++) {
      int a = p1_h1[(i - 1 + p1_h1.size()) % p1_h1.size()];
      int b = p1_h1[i];
      int c = p1_h1[(i + 1) % p1_h1.size()];
      if (commonEdges.count({min(a, b), max(a, b)}) &&
          commonEdges.count({min(b, c), max(b, c)})) {
        offspring1.push_back(b);
      }
    }
    for (int i = 0; i < p1_h2.size() - 1; i++) {
      int a = p1_h2[(i - 1 + p1_h2.size()) % p1_h2.size()];
      int b = p1_h2[i];
      int c = p1_h2[(i + 1) % p1_h2.size()];
      if (commonEdges.count({min(a, b), max(a, b)}) &&
          commonEdges.count({min(b, c), max(b, c)})) {
        offspring2.push_back(b);
      }
    }
    while(offspring1.size()>97){
      int a = rand()%offspring1.size();
      offspring1.erase(offspring1.begin()+a);
    }
    while(offspring2.size()>97){
      int a = rand()%offspring2.size();
      offspring2.erase(offspring2.begin()+a);
    }
    if (offspring1.size() == 0) {
      offspring1.push_back(p1_h1[0]);
    }
    if (offspring2.size() == 0) {
      offspring2.push_back(p1_h2[0]);
    }
    offspring1.push_back(offspring1.front());
    offspring2.push_back(offspring2.front());

    repair(&g, &offspring1, &offspring2);
    caching(g, &offspring1, &offspring2);
    int newCost = smartCalcDistance(&g, &offspring1, &offspring2);

    // Check uniqueness
    bool isUnique = true;
    for (int f : fitness) {
      if (f == newCost) {
        isUnique = false;
        break;
      }
    }

    // Replace worst if unique and better
    if (isUnique) {
      int worstIdx =
          max_element(fitness.begin(), fitness.end()) - fitness.begin();
      if (newCost < fitness[worstIdx]) {
        population1[worstIdx] = offspring1;
        population2[worstIdx] = offspring2;
        fitness[worstIdx] = newCost;
      }
    }
    if (newCost < bestCost) {
      // cout << "iteration: " << iteration << endl;
      // cout << "found better solution: " << newCost << endl;
      bestCost = newCost;
      *ham1 = offspring1;
      *ham2 = offspring2;
    }
  }

  // Print best cost
  int bestIdx = min_element(fitness.begin(), fitness.end()) - fitness.begin();
  // cout << "Best cost: " << fitness[bestIdx] << endl;
  cout << iteration << " ";
}
