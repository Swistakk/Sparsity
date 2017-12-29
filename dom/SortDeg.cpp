#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"
#include "ComputeWReach.hpp"


void Err() {
  cerr<<"Usage: ./SortDeg --in=graph.txtg --rad=radius [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./SortDeg --in=graph.txtg --rad=radius [--o=output.txt]"<<endl;
    cerr<<"o - if you want to print order in not default output file\n"; 
    return 1;
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
    
    string rad_str = flag_parser.GetFlag("rad", true);
    try {
      R = stoi(rad_str);
    } catch (...) {
      cerr<<"Error: Radius must be a positive integer\n";
    }
    
    output_file = graph_dir + "orders/" + graph_name + ".sortdeg" + rad_str + ".txt";
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
  
  vector<int> order(n);
  iota(order.begin(), order.end(), 1);
  vector<int> last_vis(n + 1);
  vector<int> dis(n + 1);
  vector<int> where_in_order(n + 1);
  vector<int> R_nei_sz(n + 1);
  vector<int> is_forb;
  for (int root = 1; root <= n; root++) {
    where_in_order[root] = -1; // hack, to make it think root is before everybody  in order
    vector<int> cluster = ComputeSingleCluster(graph, where_in_order, R, is_forb, last_vis, dis, root, root);
    R_nei_sz[root] = cluster.size();
    where_in_order[root] = 0;
  }
    
  sort(order.begin(), order.end(),
      [&](int a, int b) {
        if (R_nei_sz[a] != R_nei_sz[b]) {
          return R_nei_sz[a] > R_nei_sz[b];
        }
        return a < b;
      }
  );
  
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
}
  
  