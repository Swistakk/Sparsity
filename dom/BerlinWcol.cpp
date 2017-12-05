#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr<<"Usage: ./BerlinWcol graph.txtg"<<endl;
    return 1;
  }
  string graph_file = string(argv[1]);
//   string rad_str = string(argv[2]);
  string format = ".txtg";
  assert(graph_file.find(format) == graph_file.size() - format.size());
  string graph_name = graph_file.substr(0, (int)graph_file.size() - format.size());
  string output_file = "orders/" + graph_name + ".berlinfixed.txt";
//   int R = stoi(rad_str);
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  int n = graph.size() - 1;
  debug(n); 
  
  vector<int> wh_cc(n + 1, -1);
  vector<int> last_vis_v(n + 1);
  vector<int> last_vis_cc(n + 1);
  vector<int> last_important(n + 1);
  vector<int> parent(n + 1);
  int got = 0;
  int phase_ind = 0;
  vector<int> weak_order;
  vector<int> added_cnts;
  while ((int)weak_order.size() < n) {
    phase_ind++;
    int root = -1;
    for (int v = 1; v <= n; v++) {
      if (wh_cc[v] == -1 && (root == -1 || graph[v].size() > graph[root].size())) {
        root = v;
      }
    }
    vector<int> que{root};
    last_vis_v[root] = phase_ind;
    last_important[root] = phase_ind;
    parent[root] = -1;
    for (int ii = 0; ii < (int)que.size(); ii++) {
      int cur_v = que[ii];
      for (auto nei : graph[cur_v]) {
        if (wh_cc[nei] != -1 && last_vis_cc[wh_cc[nei]] != phase_ind) {
          last_vis_cc[wh_cc[nei]] = phase_ind;
          //last_important[nei] = phase_ind;
          last_important[cur_v] = phase_ind;
        }
        if (wh_cc[nei] == -1 && last_vis_v[nei] != phase_ind) {
          last_vis_v[nei] = phase_ind;
          parent[nei] = cur_v;
          que.PB(nei);
        }
      }
    }
    int cp_weak_order_sz = weak_order.size();
    for (int ii = (int)que.size() - 1; ii >= 0; ii--) {
      int cur_v = que[ii];
      if (last_important[cur_v] == phase_ind) {
        weak_order.PB(cur_v);
        wh_cc[cur_v] = phase_ind;
        if (parent[cur_v] != -1) {
          last_important[parent[cur_v]] = phase_ind;
        }
      }
    }
    reverse(weak_order.begin() + cp_weak_order_sz, weak_order.end());
    //added_cnts.PB(weak_order.size() - cp_weak_order_sz);
  }
  
  //assert(added_cnts == vector<int>(n, 1));
  //debug(added_cnts);
  
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : weak_order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
  
}
  
  