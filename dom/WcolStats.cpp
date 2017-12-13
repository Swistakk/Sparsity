#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"

void Err() {
  cerr<<"Usage: graph.txtg order.txt radius --mode=wcol/sizes/full [--v]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (string(argv[1]) == "--h") {
    cerr<<"Usage: graph.txtg order.txt radius --mode=wcol/sizes/full [--v]"<<endl;
    cerr<<"mode=\n";
    cerr<<"  wcol - prints just wcol\n";
    cerr<<"  sizes - prints sizes of wreaches\n";
    cerr<<"  full - prints full wreaches and their sizes\n";
    cerr<<"--v (verbose) - print \"wcol_R(order.txt) stats:\"\n";
    return 0;
  }
  if (argc != 5 && argc != 6) {
    Err();
  }
  string graph_file = string(argv[1]);
  string order_file = string(argv[2]);
  string rad_str = string(argv[3]);
  int R = stoi(rad_str);
  string mode_arg = string(argv[4]);
  string mode_pref = "--mode=";
  bool wcol_mode = false, sizes_mode = false, full_mode = false;
  if (mode_arg.substr(0, mode_pref.size()) != mode_pref) {
    Err();
  }
  string mode_flag = mode_arg.substr(mode_pref.size());
  if (mode_flag == "wcol") {
    wcol_mode = true;
  } else if (mode_flag == "sizes") {
    sizes_mode = true;
  } else if (mode_flag == "full") {
    full_mode = true;
  } else {
    Err();
  }
  if (argc == 6) {
    assert(string(argv[5]) == "--v");
    cout<<"wcol_"<<R<<"("<<order_file<<") stats: ";
  }
  
  //cout<<"read params"<<endl;
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  //cout<<"read graph"<<endl;
  int n = graph.size() - 1;
  vector<int> order, where_in_order;
  tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  
  vector<vector<int>> wreach = ComputeAllWReach(graph, where_in_order, R, {});
  int wcol = ComputeWcolFromWReach(wreach);
  vector<int> wreach_szs(n + 1);
  for (int v = 1; v <= n; v++) {
    wreach_szs[v] = ((int)wreach[v].size());
    //cout<<root<<" has col "<<my_col[root]<<endl;
    //wcol = max(wcol, (int)wreach[root].size());
  }
//   cerr<<"[";
//   for (int i = 0; i < n; i++) {
//     cerr<<wreach_szs[order[i]];
//     if (i + 1 != n) {
//       cerr<<", ";
//     }
//   }
//   cerr<<"]\n";
  sort(wreach_szs.begin(), wreach_szs.end(), greater<int>());
//   cerr<<"Biggest wreaches:\n";
//   for (int i = 0; i < min((int)wreach_szs.size(), 20); i++) {
//     cerr<<wreach_szs[i]<<", ";
//   }
//   cerr<<endl;
  if (wcol_mode) {
    cout<<wcol<<endl;
  } else if (sizes_mode) {
    cout<<"\n";
    for (int i = 0; i < n; i++) {
      cout<<reader.GetOriginalFromMapped(order[i])<<" "<<wreach_szs[order[i]]<<"\n";
    }
  } else if (full_mode) {
    cout<<"\n";
    for (int i = 0; i < n; i++) {
      cout<<reader.GetOriginalFromMapped(order[i])<<" "<<wreach_szs[order[i]]<<" [";
      for (int j = 0; j < (int)wreach[order[i]].size(); j++) {
        cout<<reader.GetOriginalFromMapped(wreach[order[i]][j]);
        if (j + 1 != (int)wreach[order[i]].size()) {
          cout<<", ";
        }
      }
      cout<<"]\n";
    }
  }
  return 0;
} 