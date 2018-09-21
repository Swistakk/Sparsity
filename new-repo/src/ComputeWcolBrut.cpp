#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "FlagParser.hpp"

void Err() {
  cerr<<"Usage: --in_g=graph.txtg --in_o=order.txt --rad=radius"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: --in_g=graph.txtg --in_o=order.txt --rad=radius"<<endl;
    cerr<<"Computes wcol just for verification purpose"<<endl;
    return 0;
  }
  string graph_file, order_file;
  int R = -1;
  try {
    FlagParser flag_parser;
    flag_parser.ParseFlags(argc, argv);
    graph_file = flag_parser.GetFlag("in_g", true);
    order_file = flag_parser.GetFlag("in_o", true);
    string rad_str = flag_parser.GetFlag("rad", true);
    try {
      R = stoi(rad_str);
    } catch (...) {
      cerr<<"Error: Radius must be a positive integer\n";
    }
    
  } catch (string err) {
    cerr<<"Error: "<<err<<endl;
    Err();
  }
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  int n = graph.size() - 1;
  vector<int> order, where_in_order;
  tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  
  vector<int> last_vis(n + 1);
  vector<int> dis(n + 1);
  int bfs_ind = 0;
  int wcol = 0;
  for (int ii = 0; ii < n; ii++) {
    int my_wreach_sz = 1;
    int from = order[ii];
    for (int jj = 0; jj < ii; jj++) {
      int to = order[jj];
      vector<int> que{from};
      bfs_ind++;
      last_vis[from] = bfs_ind;
      dis[from] = 0;
      for (int kk = 0; kk < (int)que.size(); kk++) {
        int v = que[kk];
        if (dis[v] == R) { goto End; }
        for (auto nei : graph[v]) {
          if (where_in_order[nei] < where_in_order[to]) { continue; }
          if (nei == to) {
            my_wreach_sz++;
            goto End;
          }
          if (last_vis[nei] == bfs_ind) { continue; }
          last_vis[nei] = bfs_ind;
          dis[nei] = dis[v] + 1;
          que.PB(nei);
        }
      }
      End: ;
    }
    wcol = max(wcol, my_wreach_sz);
  }
  cout<<wcol<<endl;
  return 0;
} 