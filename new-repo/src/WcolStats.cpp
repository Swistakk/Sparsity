#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "FlagParser.hpp"

void Err() {
  cerr<<"Usage: --in_g=graph.txtg --in_o=order.txt --rad=radius --mode=wcol/sizes/full [--v]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: --in_g=graph.txtg --in_o=order.txt --rad=radius --mode=wcol/sizes/full [--v]"<<endl;
    cerr<<"mode=\n";
    cerr<<"  wcol - prints just wcol\n";
    cerr<<"  sizes - prints sizes of wreaches\n";
    cerr<<"  full - prints full wreaches and their sizes\n";
    cerr<<"--v (verbose) - print \"wcol_R(order.txt) stats:\"\n";
    return 0;
  }
  string graph_file, order_file;
  bool wcol_mode = false, sizes_mode = false, full_mode = false;
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
      Err();
    }
    
    string mode_flag = flag_parser.GetFlag("mode", true);
    if (mode_flag == "wcol") {
      wcol_mode = true;
    } else if (mode_flag == "sizes") {
      sizes_mode = true;
    } else if (mode_flag == "full") {
      full_mode = true;
    } else {
      Err();
    }
    
    if (flag_parser.Exist("v")) {
      cout<<"wcol_"<<R<<"("<<order_file<<") stats: ";
    }
  } catch (string err) {
    cerr<<"Error: "<<err<<endl;
    Err();
  }
  
  //cout<<"read params"<<endl;
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  //cout<<"read graph"<<endl;
  int n = graph.size() - 1;
  vector<int> order, where_in_order;
  tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  debug(order.size(), n);
  if (order.size() != n) {
    cerr<<"Error: Order file should have "<<n<<" vertices, but has "<<order.size()<<endl;
    exit(1);
  }
  
  if (wcol_mode) {
    cout<<ComputeWcol(graph, where_in_order, R)<<endl;
    return 0;
  }
  
  vector<vector<int>> wreach = ComputeAllWReach(graph, where_in_order, R, {});
  int wcol = ComputeWcolFromWReach(wreach);
  vector<int> wreach_szs(n + 1);
  for (int v = 1; v <= n; v++) {
    wreach_szs[v] = ((int)wreach[v].size());
  }
  if (sizes_mode) {
    for (int i = 0; i < n; i++) {
      cout<<reader.GetOriginalFromMapped(order[i])<<" "<<wreach_szs[order[i]]<<"\n";
    }
  } else if (full_mode) {
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