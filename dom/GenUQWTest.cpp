#include "Headers.hpp"
#include "FilesOps.hpp"

int main(int argc, char** argv) {
  if (argc != 3) {
    cerr<<"Usage: ./GenUQWTest test.txtg ans.txt"<<endl;
    return 1;
  }
  
  int cl1 = clock(), cl2 = clock();
  int seed = cl1 + 2000 * cl2;
  debug(seed);
  srand(seed);
  int R = 1000;
  int n = rand() % R + 5; 
  int m = rand() % R + 5;
  //debug(n, m);
  m = min(1ll * m, 1ll * n * (n - 1) / 2);
  debug(n, m);
  set<pair<int, int>> es;
  ofstream gout;
  InitOfstream(gout, argv[1]);
  set<int> ends;
  while (es.size() < 2 * m) {
    int u = rand() % n + 1;
    int v = rand() % n + 1;
    if (es.count({u, v}) || u == v) { continue; }
    es.insert({u, v});
    es.insert({v, u});
    ends.insert(u);
    ends.insert(v);
    gout<<u<<" "<<v<<"\n";
    //debug(u, v);
  }
  gout.close();
  
  ofstream aout;
  InitOfstream(aout, argv[2]);
  int forb = rand() % (n / 5 + 2);
  forb = min(forb, (int)ends.size() - 1);
  int scat = rand() % (n / (int)1e2 + 2);
  scat = min(scat, (int)ends.size() - forb);
  debug(forb, scat, ends.size());
  set<int> used;
  aout<<forb<<endl;
  for (int i = 1; i <= forb; i++) {
    int v = -1;
    while (v == -1 || used.count(v) || !ends.count(v)) {
      v = rand() % n + 1;
    }
    aout<<v<<" ";
    used.insert(v);
  }
  aout<<endl<<scat<<endl;
  for (int i = 1; i <= scat; i++) {
    int v = -1;
    while (v == -1 || used.count(v) || !ends.count(v)) {
      v = rand() % n + 1;
    }
    aout<<v<<" ";
    used.insert(v);
  }
  aout<<endl;
  aout.close();
  
  return 0;
}
  