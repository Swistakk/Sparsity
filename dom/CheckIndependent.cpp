#include "Headers.hpp"
#include "ReadTxt.hpp"
#include "FilesOps.hpp"
#include "ComputeWReach.hpp"
#include "Independent.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr<<"Usage: ./CheckIndependent graph.txtg"<<endl;
    return 1;
  }
  string graph_file = string(argv[1]); 
  GraphReader reader;
  vector<vector<int>> graph = reader.ReadGraph(graph_file);
  
  vector<int> independent_coloring = Independent1Coloring(graph);
  vector<int> independent_least_degree = Independent1LeastDegree(graph);
  
  cout<<independent_coloring.size()<<","<<independent_least_degree.size()<<endl;
  
  return 0;
}