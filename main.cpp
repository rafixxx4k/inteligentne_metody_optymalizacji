#include <bits/stdc++.h>

#include "Graph.h"
#include "greedy.h"
#include "regret.h"

using namespace std;
void loadGraph(string filename, Graph *g);
void dumpResults(string outFile, Graph *g, vector<int> ham1, vector<int> ham2);
int calcDistance(Graph g, vector<int> ham1, vector<int> ham2);

int main(int argc, char *argv[]) {
  string method = argc == 4? argv[1] : "greedy";
  string inFile = argc == 4? argv[2] : "kroA200.tsp";
  string outFile = argc == 4? argv[3] : "kroA200.out";
  inFile = "in/" + inFile;
  outFile = "out/" + outFile;
  Graph graph = Graph();
  loadGraph(inFile, &graph);
  vector<int> ham1 = {}, ham2 = {};

  if(method=="greedy")
    greedy(graph, &ham1, &ham2);
  else if(method=="greedyCycle")
    greedyCycle(graph, &ham1, &ham2);
  else if(method=="regret2")
    regret2(graph, &ham1, &ham2);
  else if(method=="regretWeighted")
    regretWeighted(graph, &ham1, &ham2);

  dumpResults(outFile, &graph, ham1, ham2);
  cout<<calcDistance(graph, ham1, ham2)<<endl;
}

void loadGraph(string filename, Graph *g) {
  string temp;
  ifstream mygraph;
  mygraph.open(filename);
  getline(mygraph, temp);  // NAME
  g->name = temp.substr(temp.find(' ') + 1);
  getline(mygraph, temp);  // TYPE
  getline(mygraph, temp);  // COMMENT
  getline(mygraph, temp);  // DIMENTIONS
  g->n = atoi(temp.substr(temp.find(' ') + 1).c_str());
  getline(mygraph, temp);  // EDGE_WEIGHT_TYPE
  getline(mygraph, temp);  // NODE_COORD_SECTION
  g->v.resize(g->n, vector<int>(g->n, 0));
  g->pos.resize(g->n);
  for (int i = 0; i < g->n; i++) {
    int id, x, y;
    mygraph >> id >> x >> y;
    g->pos[i] = make_pair(x, y);
  }
  mygraph.close();
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