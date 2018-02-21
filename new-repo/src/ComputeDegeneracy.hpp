#pragma once

#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"
#include "ComputeWReach.hpp"
#include "CommonGraph.hpp"

// Computes denegeracy order on graph G where u and v are connected
// iff dis_graph(u, v) <= R
// main usage is of course R = 1
pair<int, vector<int>> Degeneracy(vector<vector<int>>& graph, int R);
  