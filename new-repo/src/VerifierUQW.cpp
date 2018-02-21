#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "CommonGraph.hpp"
#include "ComputeWReach.hpp"

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
  vector<int> is_forb(n + 1), forb_vec, scattered;
  int forb_sz, scattered_sz;
  ain >> forb_sz;
  for (int i = 1; i <= forb_sz; i++) {
    string v;
    ain >> v;
    int mapped = reader.GetMappedFromOriginal(v);
    assert(mapped != -1);
    is_forb[mapped] = 1;
    forb_vec.PB(mapped);
  }
  ain >> scattered_sz;
  for (int i = 1; i <= scattered_sz; i++) {
    string v;
    ain >> v;
    int mapped = reader.GetMappedFromOriginal(v);
    assert(mapped != -1);
    assert(is_forb[mapped] == 0);
    scattered.PB(mapped);
  }
  ain.close();
  
  const int kInf = 1e9;
  if (scattered_sz <= 1) {
    cout<<forb_sz<<","<<scattered_sz<<","<<scattered_sz<<endl;
    return 0;
  }
  vector<int> que;
  vector<int> my_root(n + 1);
  for (auto v : scattered) {
    que.PB(v);
    my_root[v] = v;
  }
  vector<int> dis(n + 1);
  int closest_pair_dis = kInf;
  int max_chk = kInf;
  for (int ii = 0; ii < (int)que.size(); ii++) {
    int cur_v = que[ii];
    for (auto nei : graph[cur_v]) {
      if (is_forb[nei]) { continue; }
      if (my_root[nei] != my_root[cur_v] && my_root[nei] != 0) {
        int cand_dis = dis[nei] + dis[cur_v] + 1;
        if (cand_dis < closest_pair_dis) {
          string orig_root_nei = reader.GetOriginalFromMapped(my_root[nei]);
          string orig_cur_v_nei = reader.GetOriginalFromMapped(my_root[cur_v]);
          debug(my_root[nei], my_root[cur_v], cand_dis, orig_root_nei, orig_cur_v_nei);
        }
        closest_pair_dis = min(closest_pair_dis, dis[nei] + dis[cur_v] + 1);
        if (max_chk == kInf) {
          max_chk = (int)que.size();
        }
      }
      if (my_root[nei] == 0) {
        my_root[nei] = my_root[cur_v];
        que.PB(nei);
        dis[nei] = dis[cur_v] + 1;
      }
    }
    if (ii >= max_chk) { break; }
  }
  debug(forb_sz, scattered_sz);
  if (closest_pair_dis > D) {
    cout<<forb_sz<<","<<scattered_sz<<","<<UQWScore(graph, D, forb_vec, scattered)<<endl;
  } else {
    cout<<"###,###,###"<<endl;
  }
  return 0;
} 