// not completed greedy domest

#include <bits/stdc++.h>

#define PB push_back
#define st first
#define nd second

using namespace std;

template<typename TH> void _dbg(const char* sdbg, TH h) { cerr<<sdbg<<"="<<h<<"\n"; }
template<typename TH, typename... TA> void _dbg(const char* sdbg, TH h, TA... t) {
  while(*sdbg != ',')cerr<<*sdbg++; cerr<<"="<<h<<","; _dbg(sdbg+1, t...);
}
#ifdef LOCAL
#define debug(...) _dbg(#__VA_ARGS__, __VA_ARGS__)
#define debugv(x) {{cerr <<#x <<" = "; FORE(itt, (x)) cerr <<*itt <<", "; cerr <<"\n"; }}
#else
#define debug(...) (__VA_ARGS__)
#define debugv(x)
#define cerr if(0)cout
#endif

void GreedyDom(vector<vector<int>> graph, int n, map<int, int>& inv_shrinking, string filename) {
  ofstream out;
  out.open(filename);
  
  vector<int> deg(n + 5);
  for (int v = 1; v <= n; v++) {
    deg[v] = graph[v].size();
  }
  vector<int> dom;
  
  
  
  
  sort(dom.begin(), dom.end());
  out<<dom.size()<<endl;
  for (auto& v : dom) {
    out<<inv_shrinking[v]<<" ";
  }
  out<<endl;
  out.close();
}
  

int main(int argc, char** argv) {
  
  ios_base::sync_with_stdio(0);
  assert(argc == 3);
  freopen(argv[1], "r", stdin);
  int maxR = stoi(string(argv[2]));
  string filename = string(argv[1]);
  string my_ext = ".my";
  assert(filename.size() >= my_ext.size() &&
      filename.substr(filename.size() - my_ext.size(), my_ext.size()) == my_ext);
  string graph_name = filename.substr(0, filename.size() - my_ext.size());
  int n, m;
  cin>>n>>m;
  
  vector<pair<int, int>> edges;
  
  
  debug(n, m, maxR, graph_name);
  vector<vector<int>> graph(n + 5);
  for (auto e : edges) {
    graph[e.st].PB(e.nd);
    graph[e.nd].PB(e.st);
  }
  
  for (int radius = 1; radius <= maxR; radius++) {
    vector<int> last_vis(n + 5, -1);
    vector<int> dist(n + 5, -1);
    vector<vector<int>> new_graph(n + 5);
    for (int st_v = 1; st_v <= n; st_v++) {
      vector<int> que{st_v};
      for (int ii = 0; ii < (int)que.size(); ii++) {
        int cur_v = que[ii];
        new_graph[st_v].PB(cur_v); // this can create a loop
        if (dist[cur_v] == radius) { continue; }
        for (auto nei : graph[cur_v]) {
          if (last_vis[nei] != st_v) {
            last_vis[nei] = st_v;
            dist[nei] = dist[cur_v] + 1;
            que.PB(nei);
          }
        }
      }
    }
    map<int, int> shrinking;
    GreedyDom(new_graph, n, shrinking, graph_name + ".dom" + to_string(radius));
  }
  return 0;
}
  