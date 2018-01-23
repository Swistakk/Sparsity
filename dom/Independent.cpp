#include "Independent.hpp"

vector<int> Independent1Coloring(vector<vector<int>>& graph) {
  pair<int, vector<int>> degeneracy = Degeneracy(graph, 1);
  vector<int> degeneracy_order = degeneracy.nd;
  int max_col = degeneracy.st + 1;
  int n = graph.size() - 1;
  vector<int> color(n + 1);
  vector<vector<int>> at_color(max_col + 1);
  for (auto v : degeneracy_order) {
    set<int> neis_cols;
    for (auto nei : graph[v]) {
      if (color[nei]) {
        neis_cols.insert(color[nei]);
      }
    }
    for (int col = 1; ; col++) {
      if (neis_cols.count(col) == 0) {
        assert(col <= max_col);
        color[v] = col;
        at_color[col].PB(v);
        break;
      }
    }
  }
  int who_biggest = 1;
  for (int col = 2; col <= max_col; col++) {
    if (at_color[col].size() > at_color[who_biggest].size()) {
      who_biggest = col;
    }
  }
  return at_color[who_biggest];
}

vector<int> Independent1LeastDegree(vector<vector<int>>& graph) {
  set<pair<int, int>> que;
  int n = (int)graph.size() - 1;
  vector<int> degree(n + 1);
  vector<bool> alive(n + 1);
  for (int v = 1; v <= n; v++) {
    degree[v] = (int)graph[v].size();
    alive[v] = true;
    que.insert({degree[v], v});
  }
  auto DecDegree = [&](int v) {
    que.erase({degree[v], v});
    degree[v]--;
    que.insert({degree[v], v});
  };
  auto Remove = [&](int v) {
    que.erase({degree[v], v});
    assert(alive[v]);
    alive[v] = false;
    for (auto nei : graph[v]) {
      if (alive[nei]) {
        DecDegree(nei);
      }
    }
  };
  vector<int> independent;
  while (!que.empty()) {
    auto least_degree_it = que.begin();
    pair<int, int> least_degree = *least_degree_it;
    int v = least_degree.nd;
    independent.PB(v);
    Remove(v);
    for (auto nei : graph[v]) {
      if (alive[nei]) {
        Remove(nei);
      }
    }
  }
  return independent;
}

// it is not explicitly used in code, but intention is to call that
// only with A being an independent set
vector<int> Independent2Tree(vector<vector<int>>& graph, vector<int> A, unordered_set<int>& forb) {
  if (A.empty()) { return {{}, {}}; }
  int a = A[0];
  vector<int> nei2_vec = RNei(graph, a, 2, forb);
  unordered_set<int> nei2_set(nei2_vec.begin(), nei2_vec.end());
  vector<int> not_conflicting, conflicting;
  for (int ii = 1; ii < (int)A.size(); ii++) {
    int v = A[ii];
    if (nei2_set.count(v)) {
      conflicting.PB(v);
    } else {
      not_conflicting.PB(v);
    }
  }
  vector<int> sol_with_a = Independent2Tree(graph, not_conflicting, forb);
  sol_with_a.PB(a);
  vector<int> sol_without_a = Independent2Tree(graph, conflicting, forb);
  if (sol_with_a.size() > sol_without_a.size()) {
    return sol_with_a;
  } else {
    return sol_without_a;
  }
}

