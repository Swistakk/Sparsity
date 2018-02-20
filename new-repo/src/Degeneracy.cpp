#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"
#include "ComputeWReach.hpp"
#include "ComputeDegeneracy.hpp"

void Err() {
  cerr<<"Usage: ./Degeneracy --in=graph.txtg --rad=radius [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./Degeneracy --in=graph.txtg --rad=radius [--o=output.txt]"<<endl;
    cerr<<"rad - for rad=1 it computes typical degeneracy,"<<endl;
    cerr<<"      for rad>1 it computes degeneracy on an auxiliary graph where"<<endl;
    cerr<<"      two vertices are connected if their distance in original graph was <=R"<<endl;
    cerr<<"      what serves as some kind of naive heuristic for wcol_R"<<endl;
    cerr<<"o - if you want to print order in not default output file\n"; 
    return 1;
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
    }
    
    output_file = graph_dir + "orders/" + graph_name + ".deg" + rad_str + ".txt";
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
  vector<vector<int>> orig_graph = reader.ReadGraph(graph_file);
  int n = (int)orig_graph.size() - 1;
  pair<int, vector<int>> res = Degeneracy(orig_graph, R);
  int degeneracy = res.st;
  vector<int> degeneracy_order = res.nd;

  debug(degeneracy);
  //cerr<<"wcol_1("<<graph_file<<")="<<degeneracy + 1<<endl;
  assert((int)degeneracy_order.size() == n);
  ofstream out;
  InitOfstream(out, output_file);
  for (int v : degeneracy_order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
}
    