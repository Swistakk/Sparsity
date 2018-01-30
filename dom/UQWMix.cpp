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
    cerr<<"Usage: ./UQWFirst graph.txtg radius tree/ld_it/ld_pow"<<endl;
    cerr<<"tree/ld_it/ld_pow - method of finding 2-independent set\n";
    cerr<<"  tree - this iterative tree approach\n";
    cerr<<"  ld_it - iterative greedy least degree on G^2\n";
    cerr<<"  ld_pow - greedy least degree on G^r\n";
    return 1;
  }
  string graph_file = string(argv[1]); 
//   string format = ".txtg";
//   assert(graph_file.find(format) == graph_file.size() - format.size());
//   string graph_name = graph_file.substr(0, (int)graph_file.size() - format.size());
  //string order_file = string(argv[2]);
  string rad_str = string(argv[2]);
  int R = stoi(rad_str);
  string mode = string(argv[3]);
  bool tree_mode = false, ld_it_mode = false, ld_pow_mode = false;
  if (mode == "tree") {
    tree_mode = true;
  } else if (mode == "ld_it") {
    ld_it_mode = true;
  } else {
    assert(mode == "ld_pow");
    ld_pow_mode = true;
  }
  
  
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  //cout<<"read graph"<<endl;
  int n = graph.size() - 1;
//   vector<int> order, where_in_order;
//   tie(order, where_in_order) = GetOrderAndWhInOrder(order_file, reader);
  
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
  if (ld_pow_mode) {
    vector<vector<int>> candsA;
    vector<int> candsAszs;
    vector<unordered_set<int>> candsS;
    vector<vector<int>> candsS_vec;
    for (int phase = 0; phase < min(10, n); phase++) {
      vector<int> independent = IndependentRLeastDegreePow(graph, oldA, R, S);
      candsA.PB(independent);
      candsAszs.PB(independent.size());
      candsS.PB(S);
      candsS_vec.PB(vector<int>(S.begin(), S.end()));
      int to_delete = 0;
      for (int v = 1; v <= n; v++) {
        if (S.count(v)) { continue; }
        if (to_delete == 0 || graph[to_delete].size() < graph[v].size()) {
          to_delete = v;
        }
      }
      S.insert(to_delete);
      for (auto& v : oldA) {
        if (v == to_delete) {
          swap(v, oldA.back());
          break;
        }
      }
      oldA.pop_back();
    }
    int who_biggest = -1;
    int biggest_score = -1;
    for (int ii = 0; ii < (int)candsA.size(); ii++) {
      int cand_score = UQWScore(graph, R, candsS_vec[ii], candsA[ii]);
      if (cand_score > biggest_score) {
        who_biggest = ii;
        biggest_score = cand_score;
      }
    }
    oldA = candsA[who_biggest];
    S = candsS[who_biggest];
  } else {
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
        // I hate these "+1"s around here
        vector<vector<int>> graph_on_balls((int)oldA.size() + 1);
        for (int ii = 0; ii < (int)oldA.size(); ii++) {
          unordered_set<int> my_neis;
          // forw_mapping[v] == ii + 1
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
            assert(branch_set_root[v] == 0);
            branch_set_root[v] = a;
          }
        }
        for (int v = 1; v <= n; v++) {
          if (S.count(v)) { continue; }
          if (branch_set_root[v] == 0) {
            branch_set_root[v] = v;
          }
          forw_mapping[branch_set_root[v]] = 1;
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
        vector<bool> is_in_B(cnt + 1);
        for (int v = 1; v <= n; v++) {
          if (is_in_ball[v]) { continue; }
          if (S.count(v)) { continue; }
          is_in_B[forw_mapping[v]] = true;
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
        vector<int> forwA_init = forwA;
        vector<vector<int>> candsA;
        vector<int> candsAszs;
        vector<unordered_set<int>> candsS;
        unordered_set<int> curS;
        while (forwA.size() > curS.size()) {
          vector<int> independent;
          if (tree_mode) {
            independent = Independent2Tree(bipartite_graph, forwA_init, curS);
          } else {
            independent = IndependentRLeastDegreePow(bipartite_graph, forwA_init, 2, curS);
          }
          candsA.PB(independent);
          candsAszs.PB(independent.size());
          candsS.PB(curS);
          int who_biggest = -1;
          int biggest_forwA_deg = 0;
          unordered_set<int> forwA_set(forwA.begin(), forwA.end());
          for (int v = 1; v <= cnt; v++) {
            if (!is_in_B[v]) { continue; }
            if (curS.count(v)) { continue; }
            int forwA_deg = 0;
            for (auto nei : bipartite_graph[v]) {
              forwA_deg += forwA_set.count(nei);
            }
            if (forwA_deg > biggest_forwA_deg) {
              biggest_forwA_deg = forwA_deg;
              who_biggest = v;
            }
          }
          if (who_biggest == -1) { break; }
          curS.insert(who_biggest);
          forwA.clear();
          for (auto nei : bipartite_graph[who_biggest]) {
            if (forwA_set.count(nei)) {
              forwA.PB(nei);
            }
          }
        }
        
        vector<vector<int>> backw_candsA;
        vector<vector<int>> backw_candsS;
        for (auto& candA : candsA) {
          vector<int> backw_candA;
          for (auto v : candA) {
            backw_candA.PB(backw_mapping[v]);
          }
          backw_candsA.PB(backw_candA);
        }
        for (auto& candS : candsS) {
          vector<int> backw_candS = vector<int>(S.begin(), S.end()); // we may try to initialize it as empty vector as well
          for (auto v : candS) {
            backw_candS.PB(backw_mapping[v]);
          }
          backw_candsS.PB(backw_candS);
        }
        
        int who_biggest = -1;
        int biggest_score = -1;
        for (int ii = 0; ii < (int)backw_candsA.size(); ii++) {
          int score = UQWScore(graph, R, backw_candsS[ii], backw_candsA[ii]); // R here could be curR + 1 as well
          if (biggest_score < score) {
            biggest_score = score;
            who_biggest = ii;
          }
        }
        oldA = backw_candsA[who_biggest];
        S = unordered_set<int>(backw_candsS[who_biggest].begin(), backw_candsS[who_biggest].end());
      }
      //cerr<<"after step "<<curR+1<<" SZ(A)="<<oldA.size()<<" "<<oldA<<endl;
    }
  }
  
  vector<int> best_forb;
  for (auto s : S) {
    best_forb.PB(s);
  }
  vector<int> best_scat = oldA;
  //debug(best_forb, best_scat);
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