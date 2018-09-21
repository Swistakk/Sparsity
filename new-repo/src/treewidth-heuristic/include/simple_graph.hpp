#ifndef SIMPLE_GRAPH_INCLUDED
#define SIMPLE_GRAPH_INCLUDED

#include <unordered_map>
// Vertex ids
typedef unsigned int Vertex;

// a graph is a vector of (ordered) sets
// we need only iterating through vertices and neighbours of a vertex
// sets are  better than vectors for reading the graph (to ignore
// already existing edges)
class Graph {
public:
  //  typedef boost::container::small_vector<Vertex, 10> VertexList;
  // Alternatively, try
  // typedef std::vector<Vertex> VertexList;
  // small_vector is more efficient if many vertices have small
  // degree.

    typedef std::set<Vertex> NeighboursSet;

  Graph(const std::vector<std::set<Vertex> >& adjacency_list = std::vector<std::set<Vertex> >(),
        const std::unordered_map<Vertex, Vertex>& vertex_reverse_map = std::unordered_map<Vertex, Vertex>()):
    vertex_to_id_map_(vertex_reverse_map) {
    for(const auto& N : adjacency_list) {
      NeighboursSet l;
      for(Vertex v : N)
        l.insert(v);
      g_.emplace_back(l);
    }
    consistency_check();
  }
  explicit Graph(std::istream& in) {
    std::string s;
    // Remap the vertices to a contiguous segment 0,1,2,...
    // id_to_vertex_map maps original vertex ids (from the istream) to
    // artificial, contiguous ids.  At the end of this function, we
    // will store the reverse mapping in vertex_to_id_.
    std::unordered_map<Vertex, Vertex> id_to_vertex_map;
    num_edges_ = 0;
    while(getline(in, s)) {
      // Skip empty lines and comments.
      if(s.length() == 0 || s[0] == '%' || s[0] == '#' || s[0] == 'c' || s[0] == 'p')
        continue;

      // The line should contain two vertex ids, describing a single
      // edge.
      Vertex a = 0, b = 0;
      const char* p = s.c_str(), *end = p + s.length();
      // Skip everything except digits.
      while(p != end && !(*p >= '0' && *p <= '9'))
        ++p;
      // Read a.
      for(; p != end && *p >= '0' && *p <= '9'; ++p)
        a = 10 * a + (*p - '0');
      // Skip everything except digits.
      while(p != end && !(*p >= '0' && *p <= '9'))
        ++p;
      // Read b.
      for(; p != end && *p >= '0' && *p <= '9'; ++p)
        b = 10 * b + (*p - '0');

      // Skip reflexive edges.
      if(a == b)
        continue;

      // skip the remainder of the line (if any)
      
      // Check if a has already been mapped to a new, artificial id.
      if(id_to_vertex_map.find(a) == id_to_vertex_map.end()) {
        // Nope, so we assign the next free id.
        id_to_vertex_map[a] = g_.size();
        a = id_to_vertex_map[a];
        g_.push_back(NeighboursSet());
      }
      else {
        // Look up the artificial id of this vertex.
        a = id_to_vertex_map[a];
      }
      // g_[a] is now the adjacency set of the new vertex.
      // Do the same thing with vertex b.
      if(id_to_vertex_map.find(b) == id_to_vertex_map.end()) {
        b = id_to_vertex_map[b] = g_.size();
        g_.push_back(NeighboursSet());
      }
      else {
        b = id_to_vertex_map[b];
      }

      if(!has_edge(a, b))
        g_[a].insert(b);
      if(!has_edge(b, a))
        g_[b].insert(a);
      num_edges_ += 2;
    }
    for(const auto& v : id_to_vertex_map)
      vertex_to_id_map_[v.second] = v.first;
  }

  std::size_t num_vertices() const { return g_.size(); }
  const NeighboursSet& neighbors(Vertex v) const { return g_[v]; }
  NeighboursSet& neighbors(Vertex v) { return g_[v]; }
  const std::size_t getDegree(Vertex v) const { return g_[v].size();}

  // Probably useful mostly for debugging because the running time is
  // O(n).
  Vertex id_to_vertex(Vertex id) const {
    for(auto i : vertex_to_id_map_)
      if(i.second == id)
        return i.first;
    throw std::out_of_range("Original vertex id out of range: " + std::to_string(id));
  }

  unsigned vertex_to_id(Vertex v) const {
    auto i = vertex_to_id_map_.find(v);
    if(i != vertex_to_id_map_.end())
      return i->second;
    else
      throw std::out_of_range("Vertex id out of range: " + std::to_string(v));
  }

  bool has_edge(Vertex a, Vertex b) const {
      return (g_[a].find(b) != g_[a].end());
    // for(Vertex v : g_[a])
    //   if(v == b)
    //     return true;
    // return false;
  }

  size_t numb_edges_dir() const {
    return num_edges_;
  }

    size_t num_edges_undir() const{
	return (num_edges_ / 2);
    }

  // Throws an exception if the data structure is internally
  // inconsistent.  This should never happen, but it can be useful if
  // an algorithm constructs or manipulates a graph structure
  // manually.
    void consistency_check() const { // TODO
    for(Vertex v = 0; v < g_.size(); ++v) {
      // Only check the existence id mappings if at least one exists.
      if(!vertex_to_id_map_.empty() &&
         vertex_to_id_map_.find(v) == vertex_to_id_map_.end())
        throw std::runtime_error("Missing reverse mapping for " + std::to_string(v));

      // Check that no reflexive edge exists.
      if(std::find(g_[v].begin(), g_[v].end(), v) != g_[v].end())
        throw std::runtime_error
          ("Reflexive edge: " + std::to_string(v) + " -> " + std::to_string(v));

      // For every edge from v to w...
      for(Vertex w : g_[v]) {
        // Check that the edge from w to v also exists and that it is
        // unique.
        auto c = std::count(g_[w].begin(), g_[w].end(), v);
        if(c == 0)
          throw std::runtime_error
            ("Unsymmetrical edge: " + std::to_string(v) + " -> " + std::to_string(w));
        else if(c > 1)
          throw std::runtime_error
            ("Duplicate edge: " + std::to_string(w) + " -> " + std::to_string(v));
      }
    }
  }


    void print(std::ostream& out){

	for (Vertex v = 0; v < g_.size(); ++v){
	    out << vertex_to_id_map_[v] << ": ";
	    for (Vertex w: g_[v])		
		out << vertex_to_id_map_[w] << " ";
	    out << "\n";
	}
	
    }
    
    void print_as_tgf(std::ostream& out){
	if (g_.size() == 0)
	    return;
	// print vertices with labels (label == id)
	for (Vertex v = 0; v< g_.size();++v)
	    out << vertex_to_id_map_[v] << " " << vertex_to_id_map_[v] << "\n";
	// print the separating #
	out << "#\n";
	// print edges
	for (Vertex v = 0; v<g_.size();++v){
	    auto start = g_[v].lower_bound(v);
	    if (v == *start) // should never happen
		++start;
	    if (start != g_[v].end())
		for (auto w = start; w != g_[v].end(); ++w )
		    out << vertex_to_id_map_[v] << " " << vertex_to_id_map_[*w] << "\n";
	}
    }
private:
  std::vector<NeighboursSet> g_;
  std::unordered_map<Vertex, unsigned> vertex_to_id_map_;
    size_t num_edges_; // (a,b) and (b,a) are both counted
};

#endif
