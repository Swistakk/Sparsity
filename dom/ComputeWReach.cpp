#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"


// vector<int> ComputeSingleWReach(vector<vector<int>>& graph,
//                                 vector<int>& where_in_order,
//                                 int root,  
//                                 int R,
//                                 vector<int> is_forb) {
//   int n = graph.size() - 1;
//   if (is_forb.empty()) { // corresponding to nothing forbidden
//     is_forb = vector<int>(n + 1);
//   }
//   vector<int> last_vis(n + 1, -1);
//   vector<int> dis(n + 1);
//   vector<vector<int>> res(n + 1);
//   if (is_forb[root]) { continue; }
//   last_vis[root] = root;
//   dis[root] = 0;
//   vector<int> que{root};
//   for (int ii = 0; ii < (int)que.size(); ii++) {
//     int cur_v = que[ii];
//     res[cur_v].PB(root);
//     if (dis[cur_v] == R) { continue; }
//     for (auto nei : graph[cur_v]) {
//       if (last_vis[nei] != root && where_in_order[nei] > where_in_order[root] && !is_forb[nei]) {
//         last_vis[nei] = root;
//         que.PB(nei);
//         dis[nei] = dis[cur_v] + 1;
//       }
//     }
//   }
//   return res;
// }

vector<vector<int>> ComputeAllWReach(vector<vector<int>>& graph,
                                  vector<int>& where_in_order,
                                  int R,
                                  vector<int> is_forb) {
  
  int n = graph.size() - 1;
  if (is_forb.empty()) { // corresponding to nothing forbidden
    is_forb = vector<int>(n + 1);
  }
  vector<int> last_vis(n + 1, -1);
  vector<int> dis(n + 1);
  vector<vector<int>> res(n + 1);
  for (int root = 1; root <= n; root++) {
    if (is_forb[root]) { continue; }
    last_vis[root] = root;
    dis[root] = 0;
    vector<int> que{root};
    for (int ii = 0; ii < (int)que.size(); ii++) {
      int cur_v = que[ii];
      res[cur_v].PB(root);
      if (dis[cur_v] == R) { continue; }
      for (auto nei : graph[cur_v]) {
        if (last_vis[nei] != root && where_in_order[nei] > where_in_order[root] && !is_forb[nei]) {
          last_vis[nei] = root;
          que.PB(nei);
          dis[nei] = dis[cur_v] + 1;
        }
      }
    }
  }
  return res;
}


vector<vector<int>> ComputeClustersFromWReach(vector<vector<int>>& wreach) {
  int n = wreach.size() - 1;
  vector<vector<int>> clusters(n + 1);
  for (int i = 1; i <= n; i++) {
    for (auto u : wreach[i]) {
      clusters[u].PB(i);
    }
  }
  return clusters;
}

int ComputeWcolFromWReach(vector<vector<int>>& wreach) {
  int wcol = 1;
  for (auto& v : wreach) {
    wcol = max(wcol, (int)v.size());
  }
  return wcol;
}

  