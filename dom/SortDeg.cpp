#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr<<"Usage: ./SortDeg graph.txtg"<<endl;
    return 1;
  }
  string graph_file = string(argv[1]);
  string format = ".txtg";
  assert(graph_file.find(format) == graph_file.size() - format.size());
  string graph_name = graph_file.substr(0, (int)graph_file.size() - format.size());
  string output_file = "orders/" + graph_name + ".sortdeg.txt";
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  int n = graph.size() - 1;
  debug(n); 
  
  vector<int> order(n);
  iota(order.begin(), order.end(), 1);
  sort(order.begin(), order.end(),
       [&](int a, int b) {
         if (graph[a].size() != graph[b].size()) {
           return graph[a].size() > graph[b].size();
         }
         return a < b;
       }
   );
  
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
}
  
  