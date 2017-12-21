#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"

void Err() {
  cerr<<"Usage: ./RandomWcol graph.txtg [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./RandomWcol graph.txtg [--o=output.txt]"<<endl;
    cerr<<"o - if you want to print order in not default output file\n"; 
    return 0;
  }
  if (argc != 2 && argc != 3) {
    Err();
    return 1;
  }
  string graph_file = string(argv[1]);
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

  string output_file = graph_dir + "orders/" + graph_name + ".random.txt";
  debug(output_file);
  if (argc == 3) {
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
  
  vector<int> rand_order(n);
  iota(rand_order.begin(), rand_order.end(), 1);
  srand(22);
  random_shuffle(rand_order.begin(), rand_order.end());
  
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : rand_order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
  
}
  
  