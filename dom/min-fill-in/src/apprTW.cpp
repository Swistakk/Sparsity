#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <vector>
#include <unordered_set>
#include <queue>
#include <algorithm> // std::includes

#include <ctime>

#include <boost/program_options.hpp>
namespace po = boost::program_options;


#include <boost/pending/disjoint_sets.hpp>
#include "../include/simple_graph.hpp"

#include "../include/vertex_queue_simple_graph.hpp"  

#include "../include/tests.hpp"

#include "../include/set_operations.hpp"

#include "../include/bucket.hpp"

#ifndef BAR_WIDTH_TW
#define BAR_WIDTH_TW 70
#endif



const char* usage="\nUSAGE:\n\n\t./appr_tw -i <filename>  [-p] [-f <vertex_id>] [-o] [-e] [-h]\n\n\twhere\n\t-i <filename>:\t\tfile <filename> should be in the *.tsv format\n\t\t\t\t(every line contains two numbers describing an edge),\n\t-o:\t\t\tprint the order found (default: no),\n\t-e: \t\t\tshow progrEss (default: no),\n\t-t:\t\t\tprint times used for computations (default: no),\n\t-c: \t\t\tperform a test,\n\t-d <level>: \t\tprint details of level (1-5) (default: 0),\n\t-w: \t\t\tprint computed width\n\t-r: \t\t\tchoose the next vertex randomly \n\t-h: \t\t\tprint this help\n\n";

class ApprTW_Simple_PO_Reader{
public:
    static bool read_program_options(int argc, 
				     char** argv, 
				     std::string& i_filename, 
				     bool& print_order,
				     unsigned& print_details,
				     bool& help,
				     bool& time,
				     bool& show_progress,
				     bool& test,
				     bool& print_width,
				     bool& choose_randomly
	)
	{
	    po::options_description desc("Allowed options",1024);
	    desc.add_options()
	      ("h,help", po::bool_switch(&help)->default_value(false),
		 "print this help and exit (default: no)")
	      ("i,input", po::value<std::string>(), "file name of the input graph")
	      ("order,o", po::bool_switch(&print_order)->default_value(false),
		 "print resulting order (default: no)")
	      ("progress,e", po::bool_switch(&show_progress)->default_value(false),
		 "show progrEss (default: no)")
	      ("d,details", po::value<int>(), "print level of details (default: 0 = none)")
	      ("t,time", po::bool_switch(&time)->default_value(false),
	       "print used time (default: no)")
	      ("c,check-order", po::bool_switch(&test)->default_value(false),
	       "check computed width of the order (default: no)")
	      ("w,width-print", po::bool_switch(&print_width)->default_value(false),
	       "print computed width of the order (default: yes)")
	      ("r,randomly-choose", po::bool_switch(&choose_randomly)->default_value(false),
		 "choose the next vertex randomly among vertices of least current back degree (default: no)")

		
		;

	    po::variables_map vm;
	    po::store(po::parse_command_line(argc, argv, desc), vm);
	    po::notify(vm);
 
	    if (vm.count("i")) {
		i_filename = vm["i"].as<std::string>();
	    } else {
		std::cerr << "Input file not specified. Terminating.\n" << usage;
		return false;
	    }

	    if (vm.count("d"))
		print_details = vm["d"].as<int>();
	    else
		print_details = 0;
 
	    return true;
	}
};

    void print_progress(unsigned x,
			unsigned num,
			std::string after_percent,
			unsigned bar_width=BAR_WIDTH_TW){
    	    std::cout.flush();
	    std::cout << "\r";
	    float progress = 1.0 - (float) x / num;
	    std::cout << "[";
	    unsigned pos = bar_width * progress;
	    for (unsigned i = 0; i < bar_width; ++i) {
		if (i < pos) std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	    }
	    std::cout << "] " << int(progress * 100.0) << "% ";
	    std::cout << after_percent;
    };


int main(int argc, char *argv[]){
  
    std::string i_filename;
    bool print_order , help, time, show_progress, test,print_width, choose_randomly;
    unsigned print_details;

    if (! ApprTW_Simple_PO_Reader::read_program_options(argc,
							argv,
							i_filename,
							print_order,
							print_details,
							help,
							time,
							show_progress,
							test,
							print_width,
							choose_randomly))
	return 1;

    if (help){
	std::cout << usage;
	return 0;
    }
    // get graph from tsv file
    if (print_details>1)
	std::cout << "Reading the graph...\n";
    std::ifstream i_file(i_filename);
    clock_t begin = clock();
    Graph g(i_file);
    if (time)
      std::cout << "Time for reading the graph: " << double(clock() - begin) / CLOCKS_PER_SEC << " seconds." << std::endl;

    if (print_details > 1){
      std::cout << "Number of vertices: " << g.num_vertices() << ".\n";
      std::cout << "Number of edges: " << g.num_edges_undir() << ".\n";
    }

    begin = clock();
    
    // DATA STRUCTURES

    // direct[v]: set of vertices reachable from v by an edge
    // via[v]: set of vertices reachable from v only via a component
    std::vector<std::set<Vertex> > direct(g.num_vertices());
    for(Vertex v=0; v<g.num_vertices(); ++v)
	direct[v] = g.neighbors(v);
	// for(Vertex w : g.neighbors(v))
	//     direct[v].insert(w);
  
    //   std::vector<std::set<Vertex> > via(g.num_vertices());


    // tree components
    std::map<Vertex,Vertex> parent;
    std::map<Vertex,int> rank;
    boost::disjoint_sets<
	boost::associative_property_map<std::map<Vertex,int> >,
	boost::associative_property_map<std::map<Vertex,Vertex> > >
	components(boost::make_assoc_property_map(rank),
		   boost::make_assoc_property_map(parent));

    // initialise components (trees)
    for(Vertex v=0;v<g.num_vertices();++v)
	components.make_set(v);

    // every component has a set of vertices in the yet unordered part
    // it is adjacent to
    std::vector<std::set<Vertex> > unord_neighb(g.num_vertices());

    // every vertex has a set of trees it is connected to.
    // only trees with maximal (wrt the sbuset relation)
    // neighbourhood in the yet not ordered part are kept
    std::vector<std::set<Vertex> > component_neighb(g.num_vertices());
        
    // priroty queue containing yet unordered vertices
    // This appears to be too slow if a vertex has many small
    // components. Use a vector of buckets below instead.
    
    // VertexQueueMinSimple queue_unordered(g);

    Bucket bucket(g);


    
    // resulting order
    std::vector<Vertex> order(g.num_vertices());
    size_t idx_first_unordered = g.num_vertices()-1;
    std::vector<size_t> back_degree(g.num_vertices());


    // MAIN FUNCTION


    /////////////////////////////////
    //////// local functions/////////
    /////////////////////////////////

    auto set_union = [](std::set<Vertex>& s1,
                   std::set<Vertex>& s2){
        std::set<Vertex> tmp_set;
        std::set_union(s1.begin(),
                       s1.end(),
                       s2.begin(),
                       s2.end(),
                       std::inserter(tmp_set,tmp_set.end())
            );
        s2.clear();
        s2 = std::move(tmp_set);
    };

    auto  get_back_degree = [&](Vertex v) -> size_t{
        std::set<Vertex> back_neighbours = direct[v];
        for (Vertex w : component_neighb[v])
            set_union(unord_neighb[w],back_neighbours);
        // if no components, return direct[v], otherwise the
        // components added v itself, so 1 should be substracted
        return(std::max(direct[v].size(),back_neighbours.size()-1)); 
    };

    
    auto resize_bucket = [&](bool print_details=false){
        unsigned full_bucket = bucket.resize(print_details);
        // recompute the buckets of the non-ordered vertices

	Bucket bucket_copy(bucket.size());
 
        for (auto vIt=bucket.begin(full_bucket); vIt != bucket.end(full_bucket); ++vIt ){
            Vertex v = *vIt;
            if (direct[v].size() >= bucket.size()-1) // filter: check direct[v].size first
                bucket_copy.insert(v,bucket.size()-1);
            else{
                size_t back_degree = get_back_degree(v);
                if (back_degree >= bucket.size()-1)
                    bucket_copy.insert(v,bucket.size()-1);
                else
                    bucket_copy.insert(v,back_degree);
            }
        }
	bucket = std::move(bucket_copy);
    };

    
    //
    // FOR DEBUGGING
    //
    // auto print_direct = [&](Vertex v){
    // 	std::cout << "\t\t\t\tdirect[" << g.vertex_to_id(v) << "]: ";
    // 	for (Vertex w : direct[v])
    // 	    std::cout << g.vertex_to_id(w) << " ";
    // 	std::cout << "\n";  
    // };
  
    auto print_vertex_set = [&](const std::set<Vertex> s){
    	for (Vertex w : s)
    	    std::cout << g.vertex_to_id(w) << " ";
    	std::cout << "\n";
    };

    // compute the difference of s1 and s2 and put the result to s1
    // auto set_diff = [](std::set<Vertex>& s1,
    // 		       std::set<Vertex>& s2){
    // 	std::set<Vertex> tmp_set;
    // 	std::set_difference(s1.begin(),
    // 			    s1.end(),
    // 			    s2.begin(),
    // 			    s2.end(),
    // 			    std::inserter(tmp_set,tmp_set.end())
    // 	    );
    // 	s1.clear();
    // 	s1 = std::move(tmp_set);
    // };
    
    // compute the union of s1 and s2 and put the result to s2
    //// end local functions ////

    if (g.num_vertices() == 0){
	std::cout << "The graph is empty.\n";
	return 0;
    }

    if (g.num_vertices() == 1){
	std::cout << "The graph contains only one vertex.\n";
	return 0;
    }

    size_t width=0;


    if (print_details > 1){//print buckets
        std::cout << "Initial buckets:\n";
        bucket.print(g);
    }

    
    for (;idx_first_unordered>0;--idx_first_unordered){//the last vertex is ordered automatically
	clock_t begin_new_vertex = clock();
	if (show_progress){
	    std::string s = "\t current width: " +
		std::to_string(width) + "\t" +
		std::to_string(g.num_vertices() - idx_first_unordered) + " vertices sorted";
	    print_progress(idx_first_unordered,g.num_vertices(),s);
	}
	// Vertex v = queue_unordered.top();
	// queue_unordered.pop();

        // alternative implementation with buckets
        // Here more intelligent heuristics than taking any element
        // may be more appropriate.

        while (bucket.need_resize(print_details)){
            resize_bucket(print_details);
	    if (print_details>3){
		bucket.print(g);
		std::cout << "\n=================\n";
	    }
	}


	Vertex v;
	if (choose_randomly)
	    v = bucket.get_least_random_vertex();
	else
	    v = bucket.get_least_vertex();
        if (print_details > 1){//debugging
	    std::cout << "CHOSE next vertex: " << g.vertex_to_id(v) << " at position " << idx_first_unordered << "\n";
            bucket.print(g);
        }
        bucket.erase(v);
        if (print_details > 1){//debugging
	    bucket.print(g);
	}   

	order[idx_first_unordered] = v;
        bool v_to_delete = false; // if the unord_neighb of the new
                                  // component of v appears to be a
                                  // subset of another component,
                                  // unord_neighb[v] must be
                                  // cleared. It won't be able to be cleared at
                                  // once because we iterate over its
                                  // neighbours.

	
	// update the width
	std::set<Vertex> via(std::move(direct[v]));
	if (print_details > 1){//debugging
	    std::cout << "Direct of " << g.vertex_to_id(v) << ": ";
	    print_vertex_set(via);
	    std::cout << "\n";

	    std::cout << "\tComponents of " << g.vertex_to_id(v) << ": ";
	    print_vertex_set(component_neighb[v]);
	    std::cout << "\n";
	}
	for (Vertex comp: component_neighb[v]){
	    set_union(unord_neighb[comp],via);
	    unord_neighb[comp].clear(); // free memory
	}
	via.erase(v);
	if (print_details > 1){//debugging
	    std::cout << "\tErased " << g.vertex_to_id(v) << " from via.\n";
	}
	size_t width_of_v = via.size();
	back_degree[idx_first_unordered] = width_of_v;
	
	width = std::max(width,width_of_v);
	
	if (print_details > 1){
	    std::cout << "\t\t Current width: " << width << "\n";
	    std::cout << "\t" << g.num_vertices() - idx_first_unordered << " vertices sorted";
        }
        
	// compute new component
	clock_t begin_merging = clock();
	for(auto& u : component_neighb[v]){
	    components.union_set(v,u);
	    if (print_details > 1){//debugging
		std::cout << "\tMerged " << g.vertex_to_id(v) << " and " << g.vertex_to_id(u) << "\n";
	    }
	}
	
	// compute new unord_neighb for the neighbours of v
	// move all unord_neighb[u] to unord_neighb[repr_v]
	Vertex repr_v = components.find_set(v);
	if (print_details > 1){//debugging
	    std::cout << "\tThe representative of " << g.vertex_to_id(v) << " is " << g.vertex_to_id(repr_v)<<"\n";
	}
	unord_neighb[repr_v] = std::move(via);
	if (v != repr_v)
	    unord_neighb[v].clear();

        // for every pair (a,b) of components, delete component a if 
        // unord_neighb[a] \subseteq unord_neighb[b]
        // it suffices to compare components of vertices in
        // unord_neighb[repr_v] with unord_neighb[repr_v]

        
	// compute new components of (direct and via) neighbours of v
	// and their back degrees
	if (time && print_details > 3){// print times
	    std::cout << "Time for merging: " << double(clock() - begin_merging) / CLOCKS_PER_SEC << " seconds." << std::endl;
        }
	if (print_details > 1){//debugging
	    std::cout << "\tUnordered neighbours of " << g.vertex_to_id(repr_v) << ": ";
	    print_vertex_set(unord_neighb[repr_v]);
	    std::cout << "\n";
	}
	for (Vertex w: unord_neighb[repr_v]){ // TODO: use unord_neighb[w] instead of direct[w]
	    if (print_details > 1){//debugging
		std::cout << "\t\tConsider neighbour " << g.vertex_to_id(w) <<"\n";
		std::cout << "\t\tcomp_neighb[" << g.vertex_to_id(w) << "]: ";
		print_vertex_set(component_neighb[w]);
                std::cout << "\t\tinsert " << g.vertex_to_id(repr_v) <<" to comp_neighb[" << g.vertex_to_id(w) << "]\n";
	    }
	    component_neighb[w].insert(repr_v);
	    // find the set of the representatives of component_neighb[w]
	    std::set<Vertex> representatives;
	    for (Vertex u : component_neighb[w]){
                Vertex repr_u = components.find_set(u);
                if (! unord_neighb[repr_u].empty())
                    representatives.insert(repr_u);
                if (repr_u != repr_v){
                    int cmp_uv = SetOperations<std::set<Vertex>>::subset(unord_neighb[repr_u].begin(),
                                                    unord_neighb[repr_u].end(),
                                                    unord_neighb[repr_v].begin(),
                                                    unord_neighb[repr_v].end());
                    if (cmp_uv == 1) // unord_neighb[repr_u] \subseteq
                                     // unord_neighb[repr_v]
                                     // => clear unord_neighb[repr_u]
                        // next time it either disappears being
                        // included into the new tree or is skipped
                        // and not inserted into representatives                        
                        unord_neighb[repr_u].clear();
                    if (cmp_uv == -1)// similar. note: this case can only
                                     // happen if v had no components,
                                     // otherwise they would have been
                                     // subsets of unord_neighb[repr_u]
                        v_to_delete = true;
                }
            }
	    component_neighb[w] = std::move(representatives);


	    // update degree of w
	    direct[w].erase(v);
            if (print_details > 1){//debugging
                std::cout << "\t\tErased " << g.vertex_to_id(v) << " from direct[" << g.vertex_to_id(w) << "].\n";
            }
            if (! bucket.known_in_last(w)){
                std::set<Vertex> via_w;
		bucket.erase(w);

                for (Vertex u : component_neighb[w]){
                    set_union(unord_neighb[u],via_w);
                    // if already clear that w is in the last bucket
                    if (via_w.size() >= bucket.size()+1){ // +1 instead of -1 for |{v,w}|
                        if (print_details > 1){//debugging
                            std::cout << "\tInserting " << g.vertex_to_id(w) << " into bucket[" << bucket.size()-1 << "].\n";
                        }
			bucket.insert(w,via_w.size());
                        break;
		    }
                }
                // actually via_w.size() - 2 < bucket.size()-1 where
                // -2 = -|{v,w}|
		if (via_w.size()  < bucket.size()+1){
		    via_w.erase(w);
		    via_w.erase(v);
	    
		    size_t direct_back = direct[w].size();
		    size_t via_back =
			SetOperations<std::set<Vertex>>::size_of_difference(via_w.begin(),
									    via_w.end(),
									    direct[w].begin(),
									    direct[w].end());

		    // queue_unordered.update_back_degree(w,direct_back+via_back);
		    size_t back_degree = direct_back + via_back;
		
		    if (back_degree + 1 > bucket.size()){
                        if (print_details > 1){//debugging
                            std::cout << "\tInserting " << g.vertex_to_id(w) << " into bucket[" << bucket.size()-1 << "].\n";
                        }
			bucket.insert(w,back_degree);
		    }
		    else {
                        if (print_details > 1){//debugging
                            std::cout << "\tInserting " << g.vertex_to_id(w) << " into bucket[" << back_degree << "].\n";
                        }
			bucket.insert(w,back_degree);
		    }
		}
            }
	}
	
        if (v_to_delete)
            unord_neighb[repr_v].clear();
	if (time && print_details){
	    std::cout << "Time for vertex " << g.vertex_to_id(v) << ": " << double(clock() - begin_new_vertex) / CLOCKS_PER_SEC << " seconds." << std::endl;
        }
    }

    // last vertex
    // Vertex v = queue_unordered.top();
    Vertex v = bucket.get_least_vertex();
    order[0] = v;
    
    if (show_progress){
	std::cout << "\r[";
	for (int i = 0; i < BAR_WIDTH_TW; ++i) 
	    std::cout << "=";
	std::cout << "] 100%\n";
	std::cout.flush();
    }
    
    if (time){
	std::cout << "Computation complete. Time: " << double(clock() - begin) / CLOCKS_PER_SEC << " seconds." << std::endl;
    }
    // print the order
    // if (print_order){
    //   std::cout<< "\n\nThe order (in brackets the back degrees): \n";
    //   for (size_t i=0; i<order.size(); ++i){
    //       // back_degree + 1 because now the vertex itself counts to
    //       // his back_degree
    //       std::cout << g.vertex_to_id(order[i]) <<"(" << back_degree[i]+1 << ") "; 
    //   }
    // 	std::cout << "\n";
    // }

    if (print_order){
	if (print_details>1)
	    std::cout<< "\n\nThe order: \n";
	for (size_t i=0; i<order.size(); ++i){
	    std::cout << g.vertex_to_id(order[i]) <<" "; 
	}
	if (print_details>1)
	    std::cout << "\n";
    }


// print the width of the order
    if (print_width){
	// width + 1 because now the vertex itself counts to his back_degree
	if (print_details>1)
	    std::cout << "The width of the order is ";
	std::cout << width+1;
	if (print_details>1)
	    std::cout << "." << std::endl;
	else
	    std::cout << std::endl;
    }
    
    // debugging
    // check if the width of the order is correct
    if (test){
        std::cout << "Testing the width of the order: ";
        if (Test::test_width(g,order,width+1,false) == width+1)
            std::cout << "test passed.\n";
        else
            std::cout << "test failed: the real width of the order is " << Test::test_width(g,order,width+1,false) << ".\n";
    }
    
    return 0;
}
