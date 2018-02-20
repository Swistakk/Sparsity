#include "ComputeDegeneracy.hpp"

pair<int, vector<int>> Degeneracy(vector<vector<int>>& graph, int R) {
  int n = graph.size() - 1;
  
  unordered_set<int> dummy_forb;
  vector<vector<int>> pow_graph = PowerGraph(graph, R, dummy_forb);
  
  int degeneracy = 0;
  vector<int> degree(n + 1);
  vector<set<int>> buckets(n + 1);
  vector<bool> already_removed(n + 1);
  set<int> nonempty_buckets;
  for (int v = 1; v <= n; v++) {
    buckets[pow_graph[v].size()].insert(v);
    nonempty_buckets.insert(pow_graph[v].size());
    degree[v] = pow_graph[v].size();
  }
  vector<int> removed_order;
  while (!nonempty_buckets.empty()) {
    int wh_bucket = *(nonempty_buckets.begin());
    degeneracy = max(degeneracy, wh_bucket);
    int v_to_remove = *(buckets[wh_bucket].begin());
    removed_order.PB(v_to_remove);
    already_removed[v_to_remove] = true;
    buckets[wh_bucket].erase(v_to_remove);
    if (buckets[wh_bucket].empty()) {
      nonempty_buckets.erase(wh_bucket);
    }
    for (auto nei : pow_graph[v_to_remove]) {
      if (already_removed[nei]) { continue; }
      buckets[degree[nei]].erase(nei);
      if (buckets[degree[nei]].empty()) {
        nonempty_buckets.erase(degree[nei]);
      }
      degree[nei]--;
      assert(degree[nei] >= 0);
      if (buckets[degree[nei]].empty()) {
        nonempty_buckets.insert(degree[nei]);
      }
      buckets[degree[nei]].insert(nei);
    }
  }
  reverse(removed_order.begin(), removed_order.end());
  return {degeneracy, removed_order};
}