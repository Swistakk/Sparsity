#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "UQWReviver.hpp"

int main(int argc, char** argv) {
  if (argc != 5) {
    cerr<<"Usage: ./UQW-MFCS graph.txtg order.txt radius percentage"<<endl;
    cerr<<"percentage - integer number from interval [0, 100] denoting how big\n";
    cerr<<"  (in percents) initial set A should be \n";
    return 1;
  }
  string graph_file = string(argv[1]);
  string format = ".txtg";
  assert(graph_file.find(format) == graph_file.size() - format.size());
  string graph_name = graph_file.substr(0, (int)graph_file.size() - format.size());
  string order_file = string(argv[2]);
  string rad_str = string(argv[3]);
  int R = stoi(rad_str);
  string percentage_str = string(argv[4]);
  int percentage = stoi(percentage_str);
//   string m_str = string(argv[4]);
//   int m = stoi(m_str);
  
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  //cout<<"read graph"<<endl;
  int n = graph.size() - 1;
  vector<int> order, where_in_order;
  tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  
  vector<int> old_A;
  int a_sz = n * percentage * .01; //n / 10;
  vector<int> rand_order(n);
  iota(rand_order.begin(), rand_order.end(), 1);
  random_shuffle(rand_order.begin(), rand_order.end());
  for (int i = 0; i < a_sz; i++) {
    old_A.PB(rand_order[i]);
  }
  
  vector<vector<int>> wreach = ComputeAllWReach(graph, where_in_order, R, {});
  //int wcol = ComputeWcolFromWReach(wreach);
  
  // works even without initial filtration, right?
  vector<int> is_forb(n + 1), forb, scat;
  while (!old_A.empty()) {
    sort(old_A.begin(), old_A.end(), [&](int a, int b) { return where_in_order[a] < where_in_order[b]; });
    assert(!old_A.empty());
    int last = old_A.back();
    scat.PB(last);
    unordered_set<int> last_wreach(wreach[last].begin(), wreach[last].end());
    vector<int> conflicting, not_conflicting;
    for (auto a : old_A) {
      if (a == last) { continue; }
      if (last_wreach.count(a)) { continue; } // <- needed because of lack of initial filtration
      bool fail = false;
      for (auto v : wreach[a]) {
        if (last_wreach.count(v)) {
          conflicting.PB(a);
          fail = true;
          break;
        }
      }
      if (!fail) {
        not_conflicting.PB(a);
      }
    }
    if (not_conflicting.size() >= conflicting.size()) {
      old_A = not_conflicting;
    } else {
      old_A = conflicting;
      for (auto to_forb : wreach[last]) {
        if (to_forb == last) { continue; }
        if (!is_forb[to_forb]) {
          is_forb[to_forb] = 1;
          forb.PB(to_forb);
        }
      }
    }
  }
  debug(forb.size(), scat.size());
  forb = ReviveRedundantForb(graph, R, forb, scat);
  debug(forb, scat);
  cout<<forb.size()<<endl;
  for (auto x : forb) {
    cout<<reader.GetOriginalFromMapped(x)<<" ";
  }
  cout<<endl<<scat.size()<<endl;
  for (auto x : scat) {
    cout<<reader.GetOriginalFromMapped(x)<<" ";
  }
  cout<<endl;
  return 0;
}