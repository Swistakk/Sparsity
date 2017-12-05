#include "ReadTxt.hpp"
#include "FilesOps.hpp"

vector<vector<int>> GraphReader::ReadGraph(string filename) {
  ifstream in;
  InitIfstream(in, filename);
  string line;
  vector<pair<int, int>> edges;
  while (getline(in, line)) {
    if (line[0] == '#') { continue; }
    stringstream stream(line);
    int a, b;
    stream >> a >> b;
    shrink_indices[a] = shrink_indices[b] = 1;
    edges.push_back({a, b});
  }
  in.close();
  int n = 0;
  for (auto& p : shrink_indices) {
    n++;
    p.nd = n;
    inv_shrinking[p.nd] = p.st;
  }
  vector<vector<int>> graph(n + 1);
  for (auto e : edges) {
    graph[shrink_indices[e.nd]].PB(shrink_indices[e.st]);
    graph[shrink_indices[e.st]].PB(shrink_indices[e.nd]);
  }
  return graph;
}

int GraphReader::GetOriginalFromMapped(int ind) {
  if (inv_shrinking.count(ind) == 0) { assert(false); }
  return inv_shrinking[ind];
}

int GraphReader::GetMappedFromOriginal(int ind) {
  if (shrink_indices.count(ind) == 0) { assert(false); }
  return shrink_indices[ind];
}