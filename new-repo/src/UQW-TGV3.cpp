#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "UQWReviver.hpp"
#include "CommonGraph.hpp"
#include "Independent.hpp"

int main(int argc, char** argv) {
  if (argc != 5) {
    cerr<<"Usage: ./UQW-TGV3 graph.txtg order.txt radius percentage"<<endl;
    cerr<<"percentage - integer number from interval [0, 100] denoting how big\n";
    cerr<<"  (in percents) initial set A should be \n";
    return 1;
  }
  string graph_file = string(argv[1]); 
//   string format = ".txtg";
//   assert(graph_file.find(format) == graph_file.size() - format.size());
//   string graph_name = graph_file.substr(0, (int)graph_file.size() - format.size());
  string order_file = string(argv[2]);
  string rad_str = string(argv[3]);
  int R = stoi(rad_str);
  string percentage_str = string(argv[4]);
  int percentage = stoi(percentage_str);
  
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  //cout<<"read graph"<<endl;
  int n = graph.size() - 1;
  vector<int> order, where_in_order;
  tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  
  vector<int> init_A;
  int a_sz = n * percentage * .01; // n / 10
  vector<int> rand_order(n);
  iota(rand_order.begin(), rand_order.end(), 1);
  random_shuffle(rand_order.begin(), rand_order.end());
  for (int i = 0; i < a_sz; i++) {
    init_A.PB(rand_order[i]);
  }
  
   //int wcol = ComputeWcolFromWReach(wreach);
  
  
  function<Solution(long double)> UQWFirst = [&](long double threshold) {
    //int db = 0.57142 < threshold && threshold < 0.57143;
    vector<vector<int>> wreach = ComputeAllWReach(graph, where_in_order, R, {});
    vector<int> old_A = init_A;
    vector<int> is_forb(n + 1), scat;
    set<int> scat_set;
    int last_forb_sz = 0;
    vector<Solution> solutions;
    set<int> scat_aside;
    
    unordered_set<int> S;
    while (!old_A.empty()) {
      sort(old_A.begin(), old_A.end(), [&](int a, int b) { return where_in_order[a] < where_in_order[b]; });
      vector<int> scat_cand = IndependentRLeastDegreePow(graph, old_A, R, S);
      for (auto v : scat_aside) {
        scat_cand.PB(v);
      }
      vector<int> S_vec(S.begin(), S.end());
      solutions.PB(Solution(graph, R, S_vec, scat_cand));
      
      set<int> Aset(old_A.begin(), old_A.end());
      int smallest_conflicts = n + 1;
      int who_smallest = -1;
      for (auto v : old_A) {
        vector<int> r_nei = RNei(graph, v, R, S);
        int count_conflicts = 0;
        for (auto nei : r_nei) {
          if (Aset.count(nei)) {
            count_conflicts++;
          }
        }
        if (count_conflicts < smallest_conflicts) {
          smallest_conflicts = count_conflicts;
          who_smallest = v;
        }
      }
      if (smallest_conflicts < 1 + threshold * (int)old_A.size()) {
        vector<int> r_nei = RNei(graph, who_smallest, R, S);
        scat_aside.insert(who_smallest);
        for (auto nei : r_nei) {
          if (Aset.count(nei)) {
            Aset.erase(nei);
          }
        }
        old_A = vector<int>(Aset.begin(), Aset.end());
      } else {
        vector<vector<int>> clusters = ComputeClustersFromWReach(wreach);
        int best_alive = -1, who_to_forb = -1;
        for (int v = 1; v <= n; v++) {
          if (is_forb[v] || scat_aside.count(v)) { continue; }
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
        S.insert(who_to_forb);
        vector<int> new_A;
        for (auto u : old_A) {
          if (u != who_to_forb) {
            new_A.PB(u);
          }
        }
        old_A = new_A;
        wreach = ComputeAllWReach(graph, where_in_order, R, is_forb);
      }
    }
    Solution best = solutions[0];
    for (int ii = 1; ii < (int)solutions.size(); ii++) {
      if (best < solutions[ii]) {
        best = solutions[ii];
      }
    }
    return best;
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