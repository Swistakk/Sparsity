#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"

int main(int argc, char** argv) {
  if (argc != 4) {
    cerr<<"Usage: ./UQWPatcher graph.txtg sol.txt radius"<<endl;
    return 1;
  }
  string graph_file = string(argv[1]);
  string sol_file = string(argv[2]);
  string rad_str = string(argv[3]);
  int R = stoi(rad_str);
  
  string format_sol = ".txt";
  assert(sol_file.find(format_sol) == sol_file.size() - format_sol.size()); 
  string output_file = sol_file.substr(0, (int)sol_file.size() - format_sol.size()) + ".p" + format_sol;
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  //cout<<"read graph"<<endl;
  int n = graph.size() - 1;
  
  ifstream sol_stream;
  InitIfstream(sol_stream, sol_file);
  vector<int> forb_mask(n + 1), scat_mask(n + 1);
  vector<int> forb, scat;
  int forb_sz, scat_sz;
  sol_stream>>forb_sz;
  for (int i = 0; i < forb_sz; i++) {
    string v_str;
    sol_stream>>v_str;
    int v = reader.GetMappedFromOriginal(v_str);
    forb_mask[v] = 1;
    forb.PB(v);
  }
  sol_stream>>scat_sz;
  for (int i = 0; i < scat_sz; i++) {
    string v_str;
    sol_stream>>v_str;
    int v = reader.GetMappedFromOriginal(v_str);
    scat_mask[v] = 1;
    scat.PB(v);
  }
  random_shuffle(forb.begin(), forb.end());
  
  vector<int> dis(n + 1);
  vector<int> last_vis(n + 1);
  int filtered_forb_sz = (int)forb.size();
  for (auto to_add : forb) {
    vector<int> que{to_add};
    last_vis[to_add] = to_add;
    dis[to_add] = 0;
    int found_scat = 0;
    int closest_guy = 0;
    bool fail = false;
    for (int ii = 0; ii < (int)que.size(); ii++) {
      int v = que[ii];
      if (found_scat == 0 && R < 2 * (dis[v] + 1)) { break; }
      if (found_scat == 1 && R < closest_guy + dis[v] + 1) { break; }
      for (auto nei : graph[v]) {
        if (forb_mask[nei]) { continue; }
        if (last_vis[nei] == to_add) { continue; }
        last_vis[nei] = to_add;
        dis[nei] = dis[v] + 1;
        que.PB(nei);
        if (scat_mask[nei]) {
          if (found_scat) {
            assert(closest_guy + dis[nei] <= R);
            fail = true;
            break;
          } else {
            closest_guy = dis[nei];
            found_scat = 1;
          }
        }
      }
      if (fail) { break; }
    }
    if (!fail) {
      forb_mask[to_add] = 0;
      filtered_forb_sz--;
    }
  }
  
  debug((int)forb.size(), filtered_forb_sz);
  
  ofstream out;
  InitOfstream(out, output_file);
  out<<filtered_forb_sz<<endl;
  for (int v = 1; v <= n; v++) {
    if (forb_mask[v]) {
      out<<reader.GetOriginalFromMapped(v)<<" ";
    }
  }
  out<<endl;
  out<<scat.size()<<endl;
  for (auto v : scat) {
    out<<reader.GetOriginalFromMapped(v)<<" ";
  }
  out<<endl;
  
  
  
  return 0;
}
  