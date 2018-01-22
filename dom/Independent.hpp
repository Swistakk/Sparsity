#pragma once

#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "FlagParser.hpp"
#include "ComputeWReach.hpp"
#include "ComputeDegeneracy.hpp"

vector<int> Independent1Coloring(vector<vector<int>>& graph);

vector<int> Independent1LeastDegree(vector<vector<int>>& graph);