// Local Search for Wcol

#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"

int main(int argc, char** argv) {
  if (argc != 4) {
    cerr<<"Usage: ./LSWcol graph.txtg order.txt radius"<<endl;
    return 1;
  }
  string graph_file = string(argv[1]); 
  string format_g = ".txtg";
  assert(graph_file.find(format_g) == graph_file.size() - format_g.size());
  string graph_name = graph_file.substr(0, (int)graph_file.size() - format_g.size());
  string order_file = string(argv[2]);
  string rad_str = string(argv[3]);
  int R = stoi(rad_str);
  string format_o = ".txt";
  assert(order_file.find(format_o) == order_file.size() - format_o.size()); 
  string output_file = order_file.substr(0, (int)order_file.size() - format_o.size()) + ".ls" + rad_str + format_o;
  
  debug(output_file);
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  int n = graph.size() - 1;
  vector<int> order, where_in_order;
  tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  
  vector<vector<int>> cur_wreach = ComputeAllWReach(graph, where_in_order, R, {});
  int best_wcol = ComputeWcolFromWReach(cur_wreach);
  vector<int> best_order = order;
  cerr<<"start wcol: "<<best_wcol<<endl;
  
  srand(22);
  int tries = 100;
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
  
  
  