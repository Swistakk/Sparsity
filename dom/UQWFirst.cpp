#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"

int main(int argc, char** argv) {
  if (argc != 4) {
    cerr<<"Usage: ./UQWFirst graph.txtg order.txt radius"<<endl;
    return 1;
  }
  string graph_file = string(argv[1]); 
//   string format = ".txtg";
//   assert(graph_file.find(format) == graph_file.size() - format.size());
//   string graph_name = graph_file.substr(0, (int)graph_file.size() - format.size());
  string order_file = string(argv[2]);
  string rad_str = string(argv[3]);
  int R = stoi(rad_str);
  
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  //cout<<"read graph"<<endl;
  int n = graph.size() - 1;
  vector<int> order, where_in_order;
  tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  
  vector<int> init_A;
  int a_sz = n; // n / 10
  vector<int> rand_order(n);
  iota(rand_order.begin(), rand_order.end(), 1);
  random_shuffle(rand_order.begin(), rand_order.end());
  for (int i = 0; i < a_sz; i++) {
    init_A.PB(rand_order[i]);
  }
  
   //int wcol = ComputeWcolFromWReach(wreach);
  
  int kl = 1, kp = init_A.size();
  vector<int> best_forb, best_scat;
  while (kl <= kp) {
    vector<vector<int>> wreach = ComputeAllWReach(graph, where_in_order, R, {});
    int m = (kl + kp) / 2;
    debug(m, kl, kp);
    vector<int> old_A = init_A;
    vector<int> is_forb(n + 1), forb, scat;
    int last_forb_sz = 0;
    while (!old_A.empty()) {
      //debug(old_A.size());
      sort(old_A.begin(), old_A.end(), [&](int a, int b) { return where_in_order[a] < where_in_order[b]; });
      int fir = old_A[0];
      set<int> Aset(old_A.begin(), old_A.end());
      unordered_set<int> fir_set;
      for (auto v : wreach[fir]) {
        fir_set.insert(v);
      }
      set<int> conflicting{fir}; // zmienic liczenie konfliktow na BFS
      for (int ii = 1; ii < (int)old_A.size(); ii++) {
        int a = old_A[ii];
        for (auto v : wreach[a]) {
          if (fir_set.count(v)) {
            conflicting.insert(a);
            break;
          }
        }
      }
      debug(conflicting.size(), old_A.size());
      if (conflicting.size() <= 1 + old_A.size() / m) { // change it
        cerr<<fir<<" into scat\n";
        scat.PB(fir);
        vector<int> new_A;
        for (auto a : old_A) {
          if (conflicting.count(a) == 0) {
            new_A.PB(a);
          }
        }
        old_A = new_A;
        last_forb_sz = forb.size();
      } else {
        vector<vector<int>> clusters = ComputeClustersFromWReach(wreach);
        int best_alive = -1, who_to_forb = -1;
        for (int v = 1; v <= n; v++) {
          if (is_forb[v] /*|| Aset.count(v)*/) { continue; }
          int count_alive = 0;
          for (auto u : clusters[v]) {
            if (u != v && Aset.count(u)) {
              count_alive++;
            }
          }
          if (count_alive > best_alive) {
            who_to_forb = v;
            best_alive = count_alive;
          }
        }
        if (best_alive == 0) {
          break;
        }
        cerr<<who_to_forb<<" into forb, alive = "<<best_alive<<endl;
        is_forb[who_to_forb] = 1;
        forb.PB(who_to_forb);
        vector<int> new_A;
        for (auto u : clusters[who_to_forb]) {
          if (u != who_to_forb && Aset.count(u)) {
            new_A.PB(u);
          }
        }
        old_A = new_A;
        wreach = ComputeAllWReach(graph, where_in_order, R, is_forb);
      }
    }
    while (forb.size() != last_forb_sz) {
      forb.pop_back();
    }
    if ((int)scat.size() >= m) {
      debug("gut");
      kl = scat.size() + 1;
      best_scat = scat;
      best_forb = forb;
    } else {
      debug("bad");
      kp = m - 1;
    }
  }
  debug(best_forb, best_scat);
  cout<<best_forb.size()<<endl;
  for (auto x : best_forb) {
    cout<<reader.GetOriginalFromMapped(x)<<" ";
  }
  cout<<endl<<best_scat.size()<<endl;
  for (auto x : best_scat) {
    cout<<reader.GetOriginalFromMapped(x)<<" ";
  }
  cout<<endl;
  return 0;
}