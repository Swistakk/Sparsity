// Local Search for Wcol

#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "FlagParser.hpp"

void Err() {
  cerr<<"Usage: ./LSWcolOld --in_g=graph.txtg --in_o=order.txt --rad=radius [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./LSWcolOld --in_g=graph.txtg --in_o=order.txt --rad=radius [--o=output.txt]"<<endl;
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
  
  vector<vector<int>> cur_wreach = ComputeAllWReach(graph, where_in_order, R, {});
  int best_wcol = ComputeWcolFromWReach(cur_wreach);
  vector<int> best_order = order;
  cerr<<"start wcol: "<<best_wcol<<endl;
  
  srand(22);
  int tries = 500;
  for (int phase = 0; phase < tries; phase++) {
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
      best_wcol = cur_wcol;
      best_order = order;
    }
  }
  
  cerr<<"new wcol: "<<best_wcol<<endl;
  
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : best_order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
  
  return 0;
}
  
  
  