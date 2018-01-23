#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "Independent.hpp"
#include "CommonGraph.hpp"
#include "ComputeDegeneracy.hpp"

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
  
  vector<int> oldA = init_A;
  unordered_set<int> S;
  for (int curR = 0; curR < R; curR++) {
    int contractR = curR / 2;
    if (curR % 2 == 0) { 
      unordered_map<int, int> forw_mapping;
      vector<int> backw_mapping((int)oldA.size() + 1);
      int cnt = 0;
      vector<vector<int>> balls((int)oldA.size());
      for (int ii = 0; ii < (int)oldA.size(); ii++) {
        int a = oldA[ii];
        balls[ii] = RNei(graph, a, contractR, S);
        for (auto v : balls[ii]) {
          assert(forw_mapping[v] == 0);
          forw_mapping[v] = ii + 1;
        }
        backw_mapping[ii + 1] = a; // backw_mapping will be basically oldA... but yeah   
      }
      vector<vector<int>> graph_on_balls((int)oldA.size() + 1);
      for (int ii = 0; ii < (int)oldA.size(); ii++) {
        unordered_set<int> my_neis;
        for (auto v : balls[ii]) {
          for (auto nei : graph[v]) {
            if (forw_mapping.count(nei) && forw_mapping[nei] != ii + 1 && my_neis.count(forw_mapping[nei]) == 0) {
              my_neis.insert(forw_mapping[nei]);
              graph_on_balls[ii + 1].PB(forw_mapping[nei]);
            }
          }
        }
      }
      vector<int> independent = Independent1LeastDegree(graph_on_balls);
      oldA.clear();
      for (auto v : independent) {
        oldA.PB(backw_mapping[v]);
      }
    } else {
      vector<vector<int>> balls((int)oldA.size());
      vector<int> forw_mapping(n + 1);
      vector<int> branch_set_root(n + 1);
      vector<bool> is_in_ball(n + 1);
      for (int ii = 0; ii < (int)oldA.size(); ii++) {
        int a = oldA[ii];
        balls[ii] = RNei(graph, a, contractR, S);
        for (auto v : balls[ii]) {
          is_in_ball[v] = true;
          assert(forw_mapping[v] == 0);
          branch_set_root[v] = a;
        }
      }
      for (int i = 1; i <= n; i++) {
        if (branch_set_root[i] == 0) {
          branch_set_root[i] = i;
        }
        forw_mapping[branch_set_root[i]] = 1;
      }
      vector<int> backw_mapping(n + 1);
      int cnt = 0;
      for (int ii = 1; ii <= n; ii++) {
        if (forw_mapping[ii] == 0) { continue; }
        cnt++;
        forw_mapping[ii] = cnt;
        backw_mapping[forw_mapping[ii]] = ii;
      }
      vector<vector<int>> bipartite_graph(cnt + 1);
      for (int v = 1; v <= n; v++) {
        if (is_in_ball[v]) { continue; }
        unordered_set<int> my_A_neis;
        for (auto nei : graph[v]) {
          if (is_in_ball[nei] && my_A_neis.count(branch_set_root[nei]) == 0) {
            my_A_neis.insert(branch_set_root[nei]);
            bipartite_graph[forw_mapping[v]].PB(forw_mapping[branch_set_root[nei]]);
            bipartite_graph[forw_mapping[branch_set_root[nei]]].PB(forw_mapping[v]);
          }
        }
      }
      vector<int> forwA;
      for (auto a : oldA) {
        forwA.PB(forw_mapping[a]);
      }
      //debug(forwA);
      vector<int> independent = Independent2Tree(bipartite_graph, forwA, S);
      //debug(independent);
      oldA.clear();
      for (auto a : independent) {
        oldA.PB(backw_mapping[a]);
      }
    }
    //cerr<<"after step "<<curR+1<<" SZ(A)="<<oldA.size()<<" "<<oldA<<endl;
  }
    
  vector<int> best_forb;
  vector<int> best_scat = oldA;
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