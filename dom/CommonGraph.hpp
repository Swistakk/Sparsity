// File for common graph operations

#pragma once

#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"
#include "ComputeWReach.hpp"

// Returns graph where u, v are connected iff dis(u, v) <= R
vector<vector<int>> PowerGraph(vector<vector<int>>& graph, int R);
  