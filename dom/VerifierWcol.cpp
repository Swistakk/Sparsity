#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"

int main(int argc, char** argv) {
  if (argc != 4) {
    cerr<<"Usage: graph.txtg order.txt radius"<<endl;
    return 1;
  }
  string graph_file = string(argv[1]);
  string order_file = string(argv[2]);
  string rad_str = string(argv[3]);
  int R = stoi(rad_str);
  
  //cout<<"read params"<<endl;
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  //cout<<"read graph"<<endl;
  int n = graph.size() - 1;
  vector<int> order;
  ifstream oin;
  InitIfstream(oin, order_file);
  vector<int> where_in_order(n + 1);
  for (int i = 1; i <= n; i++) {
    int v;
    oin >> v;
    //debug(i, v);
    int mapped = reader.GetMappedFromOriginal(v);
    assert(mapped != -1 && where_in_order[mapped] == 0);
    order.PB(mapped);
    where_in_order[mapped] = i;
  }
  oin.close();
  
  vector<vector<int>> wreach = ComputeAllWReach(graph, where_in_order, R, {});
  int wcol = ComputeWcolFromWReach(wreach);
  vector<int> wreach_szs(n + 1);
  for (int v = 1; v <= n; v++) {
    wreach_szs[v] = ((int)wreach[v].size());
    //cout<<root<<" has col "<<my_col[root]<<endl;
    //wcol = max(wcol, (int)wreach[root].size());
  }
  cerr<<"[";
  for (int i = 0; i < n; i++) {
    cerr<<wreach_szs[order[i]];
    if (i + 1 != n) {
      cerr<<", ";
    }
  }
  cerr<<"]\n";
  sort(wreach_szs.begin(), wreach_szs.end(), greater<int>());
//   cerr<<"Biggest wreaches:\n";
//   for (int i = 0; i < min((int)wreach_szs.size(), 20); i++) {
//     cerr<<wreach_szs[i]<<", ";
//   }
//   cerr<<endl;
  
  cout<<"wcol_"<<R<<"("<<order_file<<") = "<<wcol<<endl;
  return 0;
} 