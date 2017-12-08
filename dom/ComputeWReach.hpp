#pragma once

#include "Headers.hpp"

// vector<int> ComputeSingleWReach(vector<vector<int>>& graph,
//                                 vector<int>& where_in_order,
//                                 int v,  
//                                 int R,
//                                 vector<int> is_forb);

vector<vector<int>> ComputeAllWReach(vector<vector<int>>& graph,
                                  vector<int>& where_in_order,
                                  int radius,
                                  vector<int> is_forb);

vector<vector<int>> ComputeClustersFromWReach(vector<vector<int>>& wreach);

int ComputeWcolFromWReach(vector<vector<int>>& wreach);