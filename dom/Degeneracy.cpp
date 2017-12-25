#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"

void Err() {
  cerr<<"Usage: ./Degeneracy --in=graph.txtg [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./Degeneracy --in=graph.txtg [--o=output.txt]"<<endl;
    cerr<<"o - if you want to print order in not default output file\n"; 
    return 1;
  }
  string graph_file, output_file;
  try {
    FlagParser flag_parser;
    flag_parser.ParseFlags(argc, argv);
    graph_file = flag_parser.GetFlag("in", true);
    debug(graph_file);
    string format = ".txtg";
    assert(graph_file.find(format) == graph_file.size() - format.size());
    int last_slash = -1;
    for (int i = 0; i < (int)graph_file.size(); i++) {
      if (graph_file[i] == '/') {
        last_slash = i;
      }
    }
    string graph_dir = graph_file.substr(0, last_slash + 1);
    string graph_name = graph_file.substr(last_slash + 1, (int)graph_file.size() - format.size() - last_slash - 1);

    output_file = graph_dir + "orders/" + graph_name + ".deg.txt";
    debug(output_file);
    string cand_output_file = flag_parser.GetFlag("o", false);
    if (!cand_output_file.empty()) {
      output_file = cand_output_file;
      debug(output_file);
    }
    flag_parser.Close();
  } catch (string err) {
    cerr<<"Error: "<<err<<endl;
    Err();
  }
  
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
    