#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"
#include "ComputeWReach.hpp"

void Err() {
  cerr<<"Usage: ./ByWReachLeft --in=graph.txtg --rad=radius [--o=output.txt]"<<endl;
  cerr<<"--h for help\n";
  exit(1);
}

vector<int> wreach_szs;
vector<int> deg;

struct Vert {
  int id;
  bool operator<(const Vert& oth) const {
    if (wreach_szs[id] != wreach_szs[oth.id]) { return wreach_szs[id] > wreach_szs[oth.id]; }
    if (deg[id] != deg[oth.id]) { return deg[id] > deg[oth.id]; }
    return id < oth.id;
  }
};

int main(int argc, char** argv) {
  if (argc == 2 && string(argv[1]) == "--h") {
    cerr<<"Usage: ./ByWReachLeft --in=graph.txtg --rad=radius [--o=output.txt]"<<endl;
    cerr<<"o - if you want to print order in not default output file\n"; 
    return 0;
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
      Err();
    }
    
    output_file = graph_dir + "orders/" + graph_name + ".bywreach" + rad_str + ".txt";
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
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  int n = graph.size() - 1;
  debug(n);
  vector<int> where_in_order(n + 1, n + 1); // hacky hack to set where_in_order to n+1 for all not decided vertices
  vector<int> put(n + 1);
  vector<int> order;
  wreach_szs.resize(n + 1);
  deg.resize(n + 1);
  vector<int> last_vis(n + 1);
  vector<int> dis(n + 1);
  vector<int> is_forb_dummy;
  set<Vert> verts;
  for (int i = 1; i <= n; i++) {
    deg[i] = graph[i].size();
    verts.insert({i});
  }
  for (int i = 1; i <= n; i++) {
    int where_best = verts.begin()->id;
    verts.erase(verts.begin());
    where_in_order[where_best] = i;
    put[where_best] = 1;
    order.PB(where_best);
    vector<int> cluster = ComputeSingleCluster(graph, where_in_order, R, is_forb_dummy, last_vis, dis, where_best, i);
    for (auto x : cluster) {
      if (x == where_best) { continue; }
      auto it = verts.find({x});
      verts.erase(it);
      wreach_szs[x]++;
      verts.insert({x});
    }
  }
  
  ofstream out;
  InitOfstream(out, output_file);
  for (auto v : order) {
    out << reader.GetOriginalFromMapped(v) << " ";
  }
  out << endl;
  out.close();
  
}