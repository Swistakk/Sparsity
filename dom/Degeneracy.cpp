#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr<<"Usage: ./Degeneracy graph.txtg"<<endl;
    return 1;
  }
  string graph_file = string(argv[1]);
  string format = ".txtg";
  assert(graph_file.find(format) == graph_file.size() - format.size());
  string graph_name = graph_file.substr(0, (int)graph_file.size() - format.size());
  string output_file = "orders/" + graph_name + ".deg" + ".txt";
//   string output_file = "orders/" + graph_name + ".berlin" + rad_str + ".txt";
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  int n = graph.size() - 1;
  debug(n);
  
  int degeneracy = 0;
  vector<int> degree(n + 1);
  vector<set<int>> buckets(n + 1);
  vector<bool> already_removed(n + 1);
  set<int> nonempty_buckets;
  for (int v = 1; v <= n; v++) {
    buckets[graph[v].size()].insert(v);
    nonempty_buckets.insert(graph[v].size());
    degree[v] = graph[v].size();
  }
  //int fir_nonempty = 1;
  //int removed_cnt = 0;
  vector<int> removed_order;
  while (!nonempty_buckets.empty()) {
    int wh_bucket = *(nonempty_buckets.begin());
    degeneracy = max(degeneracy, wh_bucket);
    int v_to_remove = *(buckets[wh_bucket].begin());
    //debug(v_to_remove);
    removed_order.PB(v_to_remove);
    already_removed[v_to_remove] = true;
    buckets[wh_bucket].erase(v_to_remove);
    if (buckets[wh_bucket].empty()) {
      nonempty_buckets.erase(wh_bucket);
    }
    for (auto nei : graph[v_to_remove]) {
      if (already_removed[nei]) { continue; }
      buckets[degree[nei]].erase(nei);
      if (buckets[degree[nei]].empty()) {
        nonempty_buckets.erase(degree[nei]);
      }
      degree[nei]--;
      if (degree[nei] < 0) {
        debug(nei);
        graph[nei].size();
      }
      assert(degree[nei] >= 0);
      if (buckets[degree[nei]].empty()) {
        nonempty_buckets.insert(degree[nei]);
      }
      buckets[degree[nei]].insert(nei);
    }
  }
  
  debug(degeneracy);
  cerr<<"wcol_1("<<graph_file<<")="<<degeneracy + 1<<endl;
  assert((int)removed_order.size() == n);
  ofstream out;
  InitOfstream(out, output_file);
  for (int ii = n - 1; ii >= 0; ii--) {
    out << reader.GetOriginalFromMapped(removed_order[ii]) << " ";
  }
  out << endl;
  out.close();
}
    