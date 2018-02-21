#ifndef APPR_TW_TESTS
#define APPR_TW_TESTS

#include <vector>
#include <stack>

class Test{
public:
    // test if the given order has the given back degree
    // For every vertex, do dfs until a new smaller vertex is found
    static size_t test_width (Graph& g, std::vector<Vertex>& order, size_t back_degree, bool show_details){

        // compute the reverse of order
        std::vector<size_t> reverse_order(g.num_vertices());
        for (size_t i = 0; i < g.num_vertices(); ++i)
            reverse_order[order[i]] = i;

        // test reverse order
        if (show_details){
            std::cout << "The reverse order:\n";
            for (Vertex v = 0; v < g.num_vertices(); ++v )
                std::cout << "vertex " << g.vertex_to_id(v) << ": on position " << reverse_order[v] << "\n";
            std::cout<<"\n";
        }
	
        size_t width = 0;

        for (Vertex v = 0; v < g.num_vertices(); ++v){
            std::vector<bool> visited(g.num_vertices(),false);
            if (show_details)
                std::cout << "From vertex " << g.vertex_to_id(v)<< ":\n";
            std::stack<Vertex> stack;
            size_t width_of_v=1; // count the vertex itself
            stack.push(v);
            while (! stack.empty()){
                Vertex w = stack.top();
                if (show_details)
                    std::cout << "\tPop vertex " << g.vertex_to_id(w) <<"\n";
                stack.pop();
                // if (visited[w])
                //     continue;
                if (show_details)
                    std::cout << "\tVertex " << g.vertex_to_id(w) <<" is visited for the first time.\n";
                visited[w] = true;
                if (show_details)
                    std::cout << "\tMark vertex " << g.vertex_to_id(w) <<" as visited. Proceeding with its neighbours.\n";
                for (Vertex u : g.neighbors(w)){
                    if (show_details)
                        std::cout << "\t\tVertex " << g.vertex_to_id(u) <<" is a neighbour of " << g.vertex_to_id(w) << ".\n";
                    if (visited[u]){
                        continue;
                        if (show_details)
                            std::cout << "\t\tVertex " << g.vertex_to_id(u) <<" has already been visited, skipping.\n";
                    }
                    else{
                        if (show_details)
                            std::cout << "\t\tVertex " << g.vertex_to_id(u) <<" is visited for the first time.\n";
                        visited[u] = 1;
                        if (show_details)
                            std::cout << "\t\tMark vertex " << g.vertex_to_id(u) <<" as visited.\n";
                        if (reverse_order[u] >= reverse_order[v]){
                            stack.push(u);
                            if (show_details)
                                std::cout << "\t\tVertex " << g.vertex_to_id(u) <<" is >= " << g.vertex_to_id(v) << ". Push it.\n";
                        }
                        else{			
                            ++width_of_v;
                            if (show_details){
                                std::cout << "\t\tVertex " << g.vertex_to_id(u) <<" is < " << g.vertex_to_id(w) << ".\n";
                                std::cout << "\t\tNew width of " << g.vertex_to_id(v) << " is "  << width_of_v << ".\n";
                            }
                        }
                    }
                }
            }
            width = std::max(width,width_of_v);
            if (show_details)
                std::cout << "Updated width to " << width << ".\n";
        }
        // return (width == back_degree);
        return width;
    }
};

#endif
