// Local Search for Wcol

#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "FlagParser.hpp"

void Err() {
  cerr<<"Usage: ./LSWcolNew --in_g=graph.txtg --in_o=order.txt --rad=radius [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./LSWcolNew --in_g=graph.txtg --in_o=order.txt --rad=radius [--o=output.txt]"<<endl;
    cerr<<"in_o - file with order to initialize local search with\n";
    cerr<<"o - if you want to print order in not default output file\n"; 
    return 0;
  }
  string graph_file, order_file, output_file;
  int R;
  try {
    FlagParser flag_parser;
    flag_parser.ParseFlags(argc, argv);
    graph_file = flag_parser.GetFlag("in_g", true);
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
    
    order_file = flag_parser.GetFlag("in_o", true);
    string format_o = ".txt";
    assert(order_file.find(format_o) == order_file.size() - format_o.size()); 
    output_file = order_file.substr(0, (int)order_file.size() - format_o.size()) + ".ls" + rad_str + format_o;
    
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
  vector<int> order, where_in_order;
  tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  
  debug(order, where_in_order);
  
  vector<vector<int>> cur_wreach_vec = ComputeAllWReach(graph, where_in_order, R, {});
  vector<vector<int>> cur_clusters = ComputeClustersFromWReach(cur_wreach_vec);
  int best_wcol = ComputeWcolFromWReach(cur_wreach_vec);
  vector<unordered_set<int>> cur_wreach_set(n + 1);
  set<pair<int, int>> que;
  for (int i = 1; i <= n; i++) {
    for (auto v : cur_wreach_vec[i]) {
      cur_wreach_set[i].insert(v);
    }
    que.insert({cur_wreach_set[i].size(), i});
  }
  vector<int> best_order = order;
  cerr<<"start wcol: "<<best_wcol<<endl;
  
  int tries = 30000;
  vector<int> last_vis(n + 1), dis(n + 1), is_forb;
  int cnt_calls = 0;
  srand(22);
  for (int phase = 0; phase < tries; phase++) {
    auto it = prev(que.end());
    int rs = rand() % (min(10, n));
    for (int i = 0; i < rs; i++) {
      it = prev(it);
    }
    int who_worst = it->second;
    int where_worst = where_in_order[who_worst];
    assert(order[where_worst] == who_worst);
    int earlier_pos = where_worst - 1;
    int earlier_who = order[earlier_pos];
    for (auto source : vector<int>{who_worst, earlier_who}) {
      for (auto to_upd : cur_clusters[source]) {
        que.erase({cur_wreach_set[to_upd].size(), to_upd});
        cur_wreach_set[to_upd].erase(source);
        que.insert({cur_wreach_set[to_upd].size(), to_upd});
      }
    }
    swap(order[earlier_pos], order[where_worst]);
    swap(where_in_order[earlier_who], where_in_order[who_worst]);
    for (auto source : vector<int>{who_worst, earlier_who}) {
      cur_clusters[source] = ComputeSingleCluster(graph,
                                                  where_in_order,
                                                  R,
                                                  is_forb,
                                                  last_vis,
                                                  dis,
                                                  source,
                                                  ++cnt_calls);
      for (auto to_upd : cur_clusters[source]) {
        que.erase({cur_wreach_set[to_upd].size(), to_upd});
        cur_wreach_set[to_upd].insert(source);
        que.insert({cur_wreach_set[to_upd].size(), to_upd});
      }
    }
    it = prev(que.end());
    int cur_wcol = it->first;
    if (cur_wcol < best_wcol) {
      best_wcol = cur_wcol;
      best_order = order;
    }
  }
  
  cerr<<"new wcol: "<<best_wcol<<endl;
  
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
  
  return 0;
}
  
  
  