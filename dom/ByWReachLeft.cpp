#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"
#include "ComputeWReach.hpp"

void Err() {
  cerr<<"Usage: ./ByWReachLeft --in=graph.txtg --rad=radius [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./ByWReachLeft --in=graph.txtg --rad=radius [--o=output.txt]"<<endl;
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

    string rad_str = flag_parser.GetFlag("rad", true);
    try {
      R = stoi(rad_str);
    } catch (...) {
      cerr<<"Error: Radius must be a positive integer\n";
      Err();
    }
    
    output_file = graph_dir + "orders/" + graph_name + ".bywreach" + rad_str + ".txt";
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
  vector<int> where_in_order(n + 1, n + 1); // hacky hack to set where_in_order to n+1 for all not decided vertices
  vector<int> put(n + 1);
  vector<int> order;
  vector<int> wreach_szs(n + 1);
  vector<int> last_vis(n + 1);
  vector<int> dis(n + 1);
  vector<int> is_forb_dummy;
  for (int i = 1; i <= n; i++) {
    pair<int, int> best_val{-1, -1};
    int where_best = -1;
    for (int j = 1; j <= n; j++) {
      if (put[j]) { continue; }
      pair<int, int> cand = {wreach_szs[j], graph[j].size()};
      if (cand > best_val) {
        where_best = j;
        best_val = cand;
      }
    }
    where_in_order[where_best] = i;
    put[where_best] = 1;
    order.PB(where_best);
    vector<int> cluster = ComputeSingleCluster(graph, where_in_order, R, is_forb_dummy, last_vis, dis, where_best, i);
    for (auto x : cluster) {
      wreach_szs[x]++;
    }
  }
  
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
  
}