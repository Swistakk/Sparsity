// Does the following:
// while !G.empty()
//   v <- min_deg
//   make N(v) a clique
//   put v into order (from right to left)   
//   remove v

#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"

void Err() {
  cerr<<"Usage: ./ByWReachLeft --in=graph.txtg [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./ByWReachLeft --in=graph.txtg [--o=output.txt]"<<endl;
    cerr<<"o - if you want to print order in not default output file\n"; 
    return 0;
  }
  
  string graph_file, output_file;
  int R;
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
    
    output_file = graph_dir + "orders/" + graph_name + ".tw" + ".txt";
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
  vector<vector<int>> orig_graph = reader.ReadGraph(graph_file);
  int n = orig_graph.size() - 1;
  debug(n);
  vector<int> order;
  vector<int> put(n + 1);
  vector<unordered_set<int>> graph_sets(n + 1);
  for (int i = 1; i <= n; i++) {
    graph_sets[i] = unordered_set<int>(orig_graph[i].begin(), orig_graph[i].end());
    if (graph_sets[i].count(i)) {
      graph_sets[i].erase(i);
    }
  }
  for (int i = n; i >= 1; i--) {
    int where_smallest = -1;
    int smallest_deg_val = n + 1;
    for (int j = 1; j <= n; j++) {
      if (put[j]) { continue; }
      if (graph_sets[j].size() < smallest_deg_val) {
        smallest_deg_val = graph_sets[j].size();
        where_smallest = j;
      }
    }
    assert(where_smallest != -1);
    order.PB(where_smallest);
    put[where_smallest] = 1;
    
    for (auto u1 : graph_sets[where_smallest]) {
      assert(u1 != where_smallest);
      for (auto u2 : graph_sets[where_smallest]) {
        if (u1 == u2) { break; }
        if (graph_sets[u1].count(u2)) { continue; }
        graph_sets[u1].insert(u2);
        graph_sets[u2].insert(u1);
      }
    }
    
    for (auto nei : graph_sets[where_smallest]) {
      graph_sets[nei].erase(where_smallest);
    }
    graph_sets[where_smallest].clear();
  }
  
  
  reverse(order.begin(), order.end());
  
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
  
}