#include <bits/stdc++.h>

#include "Graph.h"
#include "solutions/change.h"
#include "solutions/greedy.h"
#include "solutions/local.h"
#include "solutions/regret.h"
#include "solutions/smart.h"

using namespace std;
void loadGraph(string filename, Graph *g);
void dumpResults(string outFile, Graph *g, vector<int> ham1, vector<int> ham2);
int calcDistance(Graph g, vector<int> ham1, vector<int> ham2);

int main(int argc, char *argv[]) {
  string create = argc == 5 ? argv[1] : "none";
  string method = argc == 5 ? argv[2] : "LNS";
  string inFile = argc == 5 ? argv[3] : "kroA200.tsp";
  string outFile = argc == 5 ? argv[4] : "test.out";
  inFile = "in/" + inFile;
  outFile = "out/" + outFile;
  Graph graph = Graph();
  loadGraph(inFile, &graph);
  vector<int> ham1 = {}, ham2 = {};
  if (create == "greedyNearest")
    greedyNearest(graph, &ham1, &ham2);
  else if (create == "greedyCycle")
    greedyCycle(graph, &ham1, &ham2);
  else if (create == "regret2")
    regret2(graph, &ham1, &ham2);
  else if (create == "regretWeighted")
    regretWeighted(graph, &ham1, &ham2);
  else if (create == "random")
    random(graph, &ham1, &ham2);
  else if (create == "none")
    ;

  // cout<<"lalla"<<endl;
  // cout << calcDistance(graph, ham1, ham2) << " ";
  auto start = chrono::high_resolution_clock::now();
  if (method == "localSteepVert")
    localSteepVert(graph, &ham1, &ham2);
  else if (method == "localGreedyVert")
    localGreedyVert(graph, &ham1, &ham2);
  else if (method == "localSteepEdge")
    localSteepEdge(graph, &ham1, &ham2);
  else if (method == "localGreedyEdge")
    localGreedyEdge(graph, &ham1, &ham2);
  else if (method == "candidate")
    candidate(graph, &ham1, &ham2);
  else if (method == "caching")
    caching(graph, &ham1, &ham2);
  else if (method == "MSLS")
    MSLS(graph, &ham1, &ham2);
  else if (method == "ILS")
    ILS(graph, &ham1, &ham2);
  else if (method == "LNS")
    LNS(graph, &ham1, &ham2);

  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

  dumpResults(outFile, &graph, ham1, ham2);
  cout << calcDistance(graph, ham1, ham2) << " ";
  cout << duration.count() << endl;
}

void loadGraph(string filename, Graph *g) {
  string temp;
  ifstream myGraph;
  myGraph.open(filename);
  getline(myGraph, temp);  // NAME
  g->name = temp.substr(temp.find(' ') + 1);
  getline(myGraph, temp);  // TYPE
  getline(myGraph, temp);  // COMMENT
  getline(myGraph, temp);  // DIMENSIONS
  g->n = atoi(temp.substr(temp.find(' ') + 1).c_str());
  getline(myGraph, temp);  // EDGE_WEIGHT_TYPE
  getline(myGraph, temp);  // NODE_COORD_SECTION
  g->v.resize(g->n, vector<int>(g->n, 0));
  g->pos.resize(g->n);
  for (int i = 0; i < g->n; i++) {
    int id, x, y;
    myGraph >> id >> x >> y;
    g->pos[i] = make_pair(x, y);
  }
  myGraph.close();
  for (int i = 0; i < g->n; i++) {
    for (int j = i + 1; j < g->n; j++) {
      float dist = round(sqrt(pow(g->pos[i].first - g->pos[j].first, 2.0) +
                              pow(g->pos[i].second - g->pos[j].second, 2.0)));
      g->v[i][j] = int(dist);
      g->v[j][i] = int(dist);
    }
  }
}

void dumpResults(string outFile, Graph *g, vector<int> ham1, vector<int> ham2) {
  ofstream myResult;
  myResult.open(outFile);
  myResult << g->name << '\n';
  myResult << ham1.size() << '\n';
  for (int i = 0; i < ham1.size(); i++)
    myResult << "(" << g->pos[ham1[i]].first << ";" << g->pos[ham1[i]].second
             << ") ";
  myResult << '\n' << ham2.size() << '\n';
  for (int i = 0; i < ham2.size(); i++)
    myResult << "(" << g->pos[ham2[i]].first << ";" << g->pos[ham2[i]].second
             << ") ";
  myResult.close();
}

int calcDistance(Graph g, vector<int> ham1, vector<int> ham2) {
  int dist = 0;
  for (int i = 0; i < ham1.size() - 1; i++) {
    dist += g.v[ham1[i]][ham1[i + 1]];
  }
  for (int i = 0; i < ham2.size() - 1; i++) {
    dist += g.v[ham2[i]][ham2[i + 1]];
  }
  return dist;
}