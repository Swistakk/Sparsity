#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"

void Err() {
  cerr<<"Usage: --in_g=graph.txtg --in_o=order.txt"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: --in_g=graph.txtg --in_o=order.txt"<<endl;
    return 0;
  }
  string graph_file, order_file;
  bool wcol_mode = false, sizes_mode = false, full_mode = false;
  try {
    FlagParser flag_parser;
    flag_parser.ParseFlags(argc, argv);
    graph_file = flag_parser.GetFlag("in_g", true);
    order_file = flag_parser.GetFlag("in_o", true);
  } catch (string err) {
    cerr<<"Error: "<<err<<endl;
    Err();
  }
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  int n = graph.size() - 1;
  vector<int> order, where_in_order;
  tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  set<int> in_order(order.begin(), order.end());
  
  string output_file = order_file;
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  for (int v = 1; v <= n; v++) {
    if (in_order.count(v) == 0) {
      out << reader.GetOriginalFromMapped(v) << " ";
    }
  }
  out << endl;
  out.close();
  
  
  return 0;
} 