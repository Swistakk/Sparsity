#ifndef TW_VERTEX_QUEUE_HPP
#define TW_VERTEX_QUEUE_HPP

// After Christoph Dittmann's VertexQueueMap from 16-wcol-implementations

#include <boost/heap/pairing_heap.hpp>
#include <functional>
#include "simple_graph.hpp"


class VertexQueueMinSimple {
public:
  VertexQueueMinSimple(const Graph& g = Graph()) :
    back_connectivity_(g.num_vertices()),
    degree_queue_([&](Vertex a, Vertex b) {
	    return (back_connectivity_[a] > back_connectivity_[b] ||
		    (back_connectivity_[a] == back_connectivity_[b] &&
		     g.vertex_to_id(a) > g.vertex_to_id(b)));
      }),
    degree_queue_handles_(g.num_vertices())
  {
      for (Vertex v=0; v<g.num_vertices(); ++v){
	  back_connectivity_[v] = g.getDegree(v);
	  degree_queue_handles_[v] = degree_queue_.push(v);
      }
  }

    // ~VertexQueueMinSimple(){
    //     degree_queue_.~degree_queue_type();
    // };


  bool empty() const { return degree_queue_.empty(); }
  Vertex top() const { return degree_queue_.top(); }
  std::size_t size() const { return degree_queue_.size(); }

    void pop() {
	degree_queue_.pop();
  }

    void update_back_degree(Vertex v, size_t new_degree){
    back_connectivity_[v] = new_degree;
    degree_queue_.update(degree_queue_handles_[v]);
  }

    size_t back_degree(Vertex v){return back_connectivity_[v];}

    
  // void increase_back_degree(TWGraph::Vertex v, unsigned int new_degree){
  //   back_connectivity_[v] = new_degree;
  //   degree_queue_.increase(degree_queue_handles_[v]);
  // }
  // void decrease_back_degree(TWGraph::Vertex v, unsigned int new_degree){
  //   back_connectivity_[v] = new_degree;
  //   degree_queue_.decrease(degree_queue_handles_[v]);
  // }

private:
    typedef
    boost::heap::pairing_heap<Vertex,
			      boost::heap::compare<std::function<bool (Vertex, Vertex)> > >
                      degree_queue_type;

    std::vector< std::size_t >  back_connectivity_;
    degree_queue_type degree_queue_;
    std::vector<degree_queue_type::handle_type> degree_queue_handles_;
};


#endif
