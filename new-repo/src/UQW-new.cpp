#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "UQWReviver.hpp"
#include "CommonGraph.hpp"

int main(int argc, char** argv) {
  if (argc != 5) {
    cerr<<"Usage: ./UQW-new graph.txtg order.txt radius percentage"<<endl;
    cerr<<"percentage - integer number from interval [0, 100] denoting how big\n";
    cerr<<"  (in percents) initial set A should be \n";
    return 1;
  }
  string graph_file = string(argv[1]); 
  string order_file = string(argv[2]);
  string rad_str = string(argv[3]);
  int R = stoi(rad_str);
  string percentage_str = string(argv[4]);
  int percentage = stoi(percentage_str);
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  int n = graph.size() - 1;
  vector<int> order, where_in_order;
  tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  
  vector<int> init_A;
  int a_sz = n * percentage * .01;
  vector<int> rand_order(n);
  iota(rand_order.begin(), rand_order.end(), 1);
  random_shuffle(rand_order.begin(), rand_order.end());
  for (int i = 0; i < a_sz; i++) {
    init_A.PB(rand_order[i]);
  }
  
  function<Solution(long double)> UQWFirst = [&](long double threshold) {
    vector<vector<int>> wreach = ComputeAllWReach(graph, where_in_order, R, {});
    vector<int> old_A = init_A;
    vector<int> is_forb(n + 1), forb, scat;
    set<int> scat_set;
    int last_forb_sz = 0;
    while (!old_A.empty()) {
      sort(old_A.begin(), old_A.end(), [&](int a, int b) { return where_in_order[a] < where_in_order[b]; });
      int fir = old_A[0];
      set<int> Aset(old_A.begin(), old_A.end());
      unordered_set<int> fir_set;
      for (auto v : wreach[fir]) {
        fir_set.insert(v);
      }
      set<int> conflicting{fir};
      for (int ii = 1; ii < (int)old_A.size(); ii++) {
        int a = old_A[ii];
        for (auto v : wreach[a]) {
          if (fir_set.count(v)) {
            conflicting.insert(a);
            break;
          }
        }
      }
      if (conflicting.size() <= 1 + old_A.size() * threshold) {
        scat.PB(fir);
        scat_set.insert(fir);
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
          if (is_forb[v] || scat_set.count(v)) { continue; }
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
    return Solution(graph, R, forb, scat);
  };
  
  Solution best;
  int checks = 20;
  for (int ind = 1; ind <= checks; ind++) {
    long double threshold = 1. * ind / (checks + 1);
    Solution sol = UQWFirst(threshold);
    debug(threshold, sol.score);
    if (best < sol) {
      best = sol;
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