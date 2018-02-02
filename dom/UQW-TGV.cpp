#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "UQWReviver.hpp"

struct Solution {
  vector<int> forb, scat;
  bool operator<(const Solution& oth) const { // lol
    return 1. * scat.size() / (3 + forb.size()) < 1. * oth.scat.size() / (3 + oth.forb.size()) - 1e-9; // hack to ensure halt
  }
  bool operator==(const Solution& oth) const {
    return scat.size() * (3 + oth.forb.size()) == oth.scat.size() * (3 + forb.size());
  }
};

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
  
  
  function<Solution(long double)> UQWFirst = [&](long double threshold) {
    vector<vector<int>> wreach = ComputeAllWReach(graph, where_in_order, R, {});
    vector<int> old_A = init_A;
    vector<int> is_forb(n + 1), forb, scat;
    int last_forb_sz = 0;
    while (!old_A.empty()) {
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
      //debug(conflicting.size(), old_A.size());
      if (conflicting.size() <= 1 + old_A.size() * threshold) { // change it
        //cerr<<fir<<" into scat\n";
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
        //cerr<<who_to_forb<<" into forb, alive = "<<best_alive<<endl;
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
    return Solution{forb, scat};
  };
  
  Solution best;
  long double kl = 0, kp = 1;
  int cnt = 0;
  while (1) {
    cnt++;
    long double L = (2 * kl + kp) / 3;
    long double R = (kl + 2 * kp) / 3;
    Solution sL = UQWFirst(L);
    Solution sR = UQWFirst(R);
    debug(sL.forb.size(), sL.scat.size(), sR.forb.size(), sR.scat.size());
    if (sL < sR) {
      kl = L;
      if (best < sR) {
        best = sR;
      }
    } else {
      kp = R;
      if (best < sL) {
        best = sL;
      }
      if (sL == sR) {
        if (cnt < 5) {
          kp = R; // bezsens
        } else {
          debug(L);
          break;
        }
      }
    }
  }
    
  vector<int> best_forb = best.forb;
  vector<int> best_scat = best.scat;
  best_forb = ReviveRedundantForb(graph, R, best_forb, best_scat);
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