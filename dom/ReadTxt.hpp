#pragma once

#include "Headers.hpp"

struct GraphReader {
  map<int, int> shrink_indices;
  map<int, int> inv_shrinking;
  vector<vector<int>> ReadGraph(string filename);
  int GetOriginalFromMapped(int v);
  int GetMappedFromOriginal(int v);
};