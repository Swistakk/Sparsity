#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"

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
    
    output_file = graph_dir + "orders/" + graph_name + ".bysreach" + rad_str + ".txt";
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
  vector<int> where_in_order(n + 1); // hacky hack to set where_in_order to 0 for all not decided vertices
  vector<int> put(n + 1);
  vector<int> order;
  //vector<int> cur_sreach_sz(n + 1);
  vector<unordered_set<int>> cur_sreach(n + 1);
  for (int i = 1; i <= n; i++) {
    cur_sreach[i] = unordered_set<int>(graph[i].begin(), graph[i].end());
    //assert(cur_sreach[i].count(i) == 0);
    cur_sreach[i].insert(i);
  
  }
  vector<int> last_vis(n + 1);
  vector<int> dis(n + 1);
  for (int i = n; i >= 1; i--) {
    pair<int, int> best_val{n + 1, n + 1};
    int where_best = -1;
    for (int cand = 1; cand <= n; cand++) {
      if (put[cand]) { continue; }
      pair<int, int> val = {cur_sreach[cand].size(), graph[cand].size()};
      if (val < best_val) {
        where_best = cand;
        best_val = val;
      }
    }
    //debug(where_best, reader.GetOriginalFromMapped(where_best), best_val.st, best_val.nd);
    where_in_order[where_best] = i;
    put[where_best] = 1;
    order.PB(where_best);
    vector<vector<int>> at_dist(R + 2);
    vector<int> que{where_best};
    last_vis[where_best] = where_best;
    dis[where_best] = 0;
    for (int ii = 0; ii < que.size(); ii++) {
      int v = que[ii];
      //at_dist[dis[v]].PB(v);
      for (auto nei : graph[v]) {
        if (last_vis[nei] == where_best) { continue; }
        last_vis[nei] = where_best;
        dis[nei] = dis[v] + 1;
        if (where_in_order[nei] == 0) {
          at_dist[dis[nei]].PB(nei);
          cur_sreach[nei].erase(where_best);
        } else {
          if (dis[nei] < R) {
            que.PB(nei);
          }
        }
      }
    }
    for (int r1 = 1; r1 <= R - 1; r1++) {
      for (int r2 = 1; r1 + r2 <= R; r2++) {
        for (auto v1 : at_dist[r1]) {
          for (auto v2 : at_dist[r2]) {
            if (v1 != v2) {
              cur_sreach[v1].insert(v2);
            }
          }
        }
      }
    }
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