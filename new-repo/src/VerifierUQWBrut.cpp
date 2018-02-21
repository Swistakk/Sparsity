#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"

int main(int argc, char** argv) {
  if (argc != 4) {
    cerr<<"Usage: graph.txtg ans.txt dis"<<endl;
    return 1;
  }
  string graph_file = string(argv[1]);
  string ans_file = string(argv[2]);
  string dis_str = string(argv[3]);
  int D = stoi(dis_str);
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  int n = graph.size() - 1;
  vector<int> order;
  ifstream ain;
  InitIfstream(ain, ans_file);
  vector<int> is_forb(n + 1), scattered, is_scattered(n + 1);
  int forb_sz, scattered_sz;
  ain >> forb_sz;
  for (int i = 1; i <= forb_sz; i++) {
    string v;
    ain >> v;
    int mapped = reader.GetMappedFromOriginal(v);
    assert(mapped != -1);
    is_forb[mapped] = 1;
  }
  ain >> scattered_sz;
  for (int i = 1; i <= scattered_sz; i++) {
    string v;
    ain >> v;
    int mapped = reader.GetMappedFromOriginal(v);
    assert(mapped != -1);
    assert(is_forb[mapped] == 0);
    scattered.PB(mapped);
    is_scattered[mapped] = 1;
  }
  ain.close();
  
  const int kInf = 1e9;
  int closest_pair_dis = kInf;
  if (scattered_sz <= 1) {
    cout << kInf << endl;
    return 0;
  }
  for (auto root : scattered) {
    vector<int> dis(n + 1);
    vector<int> que{root};
    vector<int> vis(n + 1);
    vis[root] = 1;
    bool found_pair = false;
    for (int ii = 0; ii < (int)que.size(); ii++) {
      int cur_v = que[ii];
      for (auto nei : graph[cur_v]) {
        if (is_scattered[nei] && nei != root) {
          closest_pair_dis = min(closest_pair_dis, dis[cur_v] + 1);
          found_pair = true;
          break;
        }
        if (!is_forb[nei] && !vis[nei]) {
          vis[nei] = 1;
          dis[nei] = dis[cur_v] + 1;
          que.PB(nei);
        }
      }
      if (found_pair) { break; }
    }
    if (!found_pair) { debug(root); }
  }
  cout << closest_pair_dis << endl;
  assert(closest_pair_dis > D);
  return 0;
} 