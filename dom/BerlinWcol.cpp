#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"

void Err() {
  cerr<<"Usage: ./BerlinWcol graph.txtg --order=bfs/dfs --rev=no/yes [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./BerlinWcol graph.txtg --order=bfs/dfs --rev=no/yes [--o=output.txt]"<<endl;
    cerr<<"order=\n";
    cerr<<"  bfs - considers ordering vertices from new blob in bfs order\n";
    cerr<<"  dfs - or in dfs order\n";
    cerr<<"rev=\n";
    cerr<<"  no - puts vertices in order in specified order\n";
    cerr<<"  yes - reverses this order\n";
    cerr<<"o - if you want to print order in not default output file\n"; 
    return 0;
  }
  if (argc != 4 && argc != 5) {
    Err();
    return 1;
  }
  string graph_file = string(argv[1]);
  string format = ".txtg";
  assert(graph_file.find(format) == graph_file.size() - format.size());
  int last_slash = -1;
  for (int i = 0; i < graph_file.size(); i++) {
    if (graph_file[i] == '/') {
      last_slash = i;
    }
  }
  string graph_dir = graph_file.substr(0, last_slash + 1);
  string graph_name = graph_file.substr(last_slash + 1, (int)graph_file.size() - format.size() - last_slash - 1);
  string order_arg = string(argv[2]);
  string order_pref = "--order=";
  bool bfs_order = false, dfs_order = false;
  if (order_arg.substr(0, order_pref.size()) != order_pref) {
    Err();
  }
  string order_flag = order_arg.substr(order_pref.size());
  if (order_flag == "bfs") {
    bfs_order = true;
  } else if (order_flag == "dfs") {
    dfs_order = true;
  } else {
    Err();
  }
  string rev_arg = string(argv[3]);
  string rev_pref = "--rev=";
  bool rev = false;
  if (rev_arg.substr(0, rev_pref.size()) != rev_pref) {
    Err();
  }
  string rev_flag = rev_arg.substr(rev_pref.size());
  if (rev_flag == "yes") {
    rev = true;
  } else if (rev_flag != "no") {
    Err();
  }
  string output_file = graph_dir + "orders/" + graph_name + ".berlin." + "bd"[dfs_order] + "ny"[rev] + ".txt";
  debug(output_file);
  if (argc == 5) {
    string o_arg = string(argv[4]);
    string o_pref = "--o";
    if (o_arg.substr(0, o_pref.size()) != o_pref) {
      Err();
    }
    output_file = o_arg.substr(o_pref.size());
  }
  
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
//     debug(reader.GetOriginalFromMapped(root));
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
    
    vector<int> blob;
    for (int ii = (int)que.size() - 1; ii >= 0; ii--) {
      int cur_v = que[ii];
      if (last_important[cur_v] == phase_ind) {
        wh_cc[cur_v] = phase_ind;
        if (parent[cur_v] != -1) {
          last_important[parent[cur_v]] = phase_ind;
        }
      }
    }
    if (bfs_order) {
      for (int ii = 0; ii < (int)que.size(); ii++) {
        int cur_v = que[ii];
        if (last_important[cur_v] == phase_ind) {
          blob.PB(cur_v);
        }
      }
    } else {
      assert(dfs_order);
      function<void(int)> Dfs = [&](int v) {
        blob.PB(v);
        last_important[v] = -1;
        for (auto nei : graph[v]) {
          if (last_important[nei] != phase_ind) { continue; }
          Dfs(nei);
        }
      };
      Dfs(root);
    }
    //debug(blob);
//     cerr<<"blob: ";
//     for (auto v : blob) {
//       cerr<<reader.GetOriginalFromMapped(v)<<", ";
//     }
//     cerr<<endl;;
    if (rev) {
      reverse(blob.begin(), blob.end());
    }
    weak_order.insert(weak_order.end(), blob.begin(), blob.end());
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
  
  