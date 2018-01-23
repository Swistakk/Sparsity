// File for common graph operations

#include "CommonGraph.hpp"

// if performance critical and used many times - it can be rewritten with
// vectors instead of maps and sets
vector<int> RNei(vector<vector<int>>& graph, int root, int R, unordered_set<int>& forb) {
  unordered_map<int, int> dis;
  vector<int> que{root};
  dis[root] = 0;
  assert(forb.count(root) == 0);
  for (int ii = 0; ii < (int)que.size(); ii++) {
    int v = que[ii];
    if (dis[v] == R) { return que; }
    for (auto nei : graph[v]) {
      if (dis.count(nei)) { continue; }
      if (forb.count(nei)) { continue; }
      dis[nei] = dis[v] + 1;
      que.PB(nei);
    }
  }
  return que; // should happen only if whole graph is reachable in less than R steps
}

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
  