// File for common graph operations

#include "CommonGraph.hpp"

// Returns graph where u, v are connected iff dis(u, v) <= R
vector<vector<int>> PowerGraph(vector<vector<int>>& graph, int R) {
  int n = (int)graph.size() - 1;
  vector<vector<int>> pow_graph(n + 1);
  vector<int> last_vis(n + 1);
  vector<int> dis(n + 1);
  vector<int> where_in_order(n + 1);
  vector<int> is_forb;
  for (int root = 1; root <= n; root++) {
    where_in_order[root] = -1; // hack, to make it think root is before everybody  in order
    pow_graph[root] = ComputeSingleCluster(graph, where_in_order, R, is_forb, last_vis, dis, root, root);
    where_in_order[root] = 0;
  }
  return pow_graph;
}
  