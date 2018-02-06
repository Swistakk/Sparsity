// Local Search for Wcol

#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "FlagParser.hpp"

void Err() {
  cerr<<"Usage: ./LSWcolBoth --in_g=graph.txtg --in_o=order.txt --rad=radius --T=timeout_secs [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

const int kMinOldTries = 400;
const int kMinNewTries = 5000;

double Timestamp() {
  return 1. * clock() / CLOCKS_PER_SEC;
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./LSWcolBoth --in_g=graph.txtg --in_o=order.txt --rad=radius --T=timeout_secs [--o=output.txt]"<<endl;
    cerr<<"in_o - file with order to initialize local search with\n";
    cerr<<"T - time (in seconds) after which graceful termination is performed\n";
    cerr<<"    if T=-1 then no such timeout is specified and algorithm works as long as it thinks\n";
    cerr<<"    there is any chance for improvement\n";
    cerr<<"o - if you want to print order in not default output file\n"; 
    return 0;
  }
  string graph_file, order_file, output_file;
  int R;
  int timeout_secs = -1;
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
      if (R <= 0) { throw 1; }
    } catch (...) {
      cerr<<"Error: Radius must be a positive integer\n";
      Err();
    }
    
    string timeout_secs_str = flag_parser.GetFlag("T", true);
    try {
      timeout_secs = stoi(timeout_secs_str);
      if (timeout_secs == -1) {
        timeout_secs = 2e9;
      }
      if (timeout_secs <= 0) { throw 1; }
    } catch (...) {
      cerr<<"Error: Timeout must be a positive integer or -1\n";
      Err();
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
  
  //debug(order, where_in_order);
  
  vector<vector<int>> cur_wreach = ComputeAllWReach(graph, where_in_order, R, {});
  int best_wcol = ComputeWcolFromWReach(cur_wreach);
  vector<int> best_order = order;
  vector<int> best_where_in_order = where_in_order;
  cerr<<"start wcol: "<<best_wcol<<endl;
  
  srand(22);
  int phase = 0;
  int last_old_update = 0;
  double old_timeout = 2. / 3 * timeout_secs;
  while (phase < max(kMinOldTries, 2 * last_old_update) && Timestamp() < old_timeout) {
    int who_worst = 1;
    for (int v = 1; v <= n; v++) {
      if (cur_wreach[v].size() >= cur_wreach[who_worst].size()) {
        who_worst = v;
      }
    }
    int where_worst = where_in_order[who_worst];
    assert(order[where_worst] == who_worst);
    int earlier_pos = rand() % where_worst;
    int earlier_who = order[earlier_pos];
    swap(order[earlier_pos], order[where_worst]);
    swap(where_in_order[earlier_who], where_in_order[who_worst]);
    cur_wreach = ComputeAllWReach(graph, where_in_order, R, {});
    int cur_wcol = ComputeWcolFromWReach(cur_wreach);
    if (cur_wcol < best_wcol) {
      debug(phase, cur_wcol);
      last_old_update = phase;
      best_wcol = cur_wcol;
      best_order = order;
      best_where_in_order = where_in_order;
    }
    phase++;
  }
  
  cerr<<"after old wcol: "<<best_wcol<<endl;
  
  order = best_order;
  where_in_order = best_where_in_order;
  
  vector<vector<int>> cur_wreach_vec = ComputeAllWReach(graph, where_in_order, R, {});
  vector<vector<int>> cur_clusters = ComputeClustersFromWReach(cur_wreach_vec);
  vector<int> cur_wreach_sz(n + 1);
  set<pair<int, int>> que;
  for (int i = 1; i <= n; i++) {
    cur_wreach_sz[i] = cur_wreach_vec[i].size();
    que.insert({cur_wreach_sz[i], i});
  }
 
  vector<int> last_vis(n + 1), dis(n + 1), is_forb;
  int cnt_calls = 0;
  int last_new_update = 0;
  srand(22);
  while (phase < max(kMinNewTries, 2 * last_new_update) && Timestamp() < timeout_secs) {
    auto it = prev(que.end());
    int rs = rand() % (min(10, n));
    for (int i = 0; i < rs; i++) {
      it = prev(it);
    }
    int who_worst = it->second;
    int target = max(1, it->first - 1 - rand() % 3);
    while (cur_wreach_sz[who_worst] > target) {
      int where_worst = where_in_order[who_worst];
      assert(order[where_worst] == who_worst);
      int earlier_pos = where_worst - 1;
      int earlier_who = order[earlier_pos];
      for (auto source : vector<int>{who_worst, earlier_who}) {
        for (auto to_upd : cur_clusters[source]) {
          que.erase({cur_wreach_sz[to_upd], to_upd});
          cur_wreach_sz[to_upd]--;
          que.insert({cur_wreach_sz[to_upd], to_upd});
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
          que.erase({cur_wreach_sz[to_upd], to_upd});
          cur_wreach_sz[to_upd]++;
          que.insert({cur_wreach_sz[to_upd], to_upd});
        }
      }
      it = prev(que.end());
      int cur_wcol = it->first;
      if (cur_wcol < best_wcol) {
        last_new_update = phase;
        best_wcol = cur_wcol;
        best_order = order;
        best_where_in_order = where_in_order;
        debug(phase);
      }
    }
    phase++;
  }
  
  cerr<<"after new wcol: "<<best_wcol<<endl;
  
  vector<vector<int>> final_wreach = ComputeAllWReach(graph, best_where_in_order, R, {});
  int final_wcol = ComputeWcolFromWReach(final_wreach);
  debug(final_wcol);
  //assert(best_wcol == final_wcol);
  
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : best_order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
  
  return 0;
}
  
  
  