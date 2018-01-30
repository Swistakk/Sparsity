#ifndef BUCKET_INCLUDED
#define BUCKET_INCLUDED

#include <stdlib.h> // random

// vector of buckets for widths of unordered vertices
// bucket[i] is the set of vertices of current back degree i
// the last element bucket[bucket.size()-1] contains the class of
// vertices with current back degree at least bucket[size()-1].
// The size of bucket grows if necessary: if all buckets but the
// last one are empty.
class Bucket{
public:
    Bucket(Graph& g):
        bucket(3)
    {
        for (Vertex v = 0; v<g.num_vertices(); ++v){
            if (g.getDegree(v) > 1){ // at least 2
                bucket[2].insert(v);
                vrtx_to_bucket[v] = 2;
            }
            else{
                bucket[g.getDegree(v)].insert(v); // 0 or 1
                vrtx_to_bucket[v] = g.getDegree(v);
            }
        }
    
        if (!bucket[0].empty())
            least_bucket = 0;
        else {
            if (!bucket[1].empty())
                least_bucket = 1;
            else
                least_bucket = 2;
        }
    }

    Bucket(unsigned size):
        bucket(size){
        least_bucket = size;
    };

    size_t size(){
	return (bucket.size());
    }
    
    void insert(Vertex v, size_t back_degree){
        if (back_degree >= bucket.size()-1){
            bucket[bucket.size()-1].insert(v);
	    lb_back_degree[v] = back_degree - bucket.size() + 1;
	    vrtx_to_bucket[v] = bucket.size()-1;
	}
        else{
            bucket[back_degree].insert(v);
	    lb_back_degree.erase(v);
	    vrtx_to_bucket[v] = back_degree;
            least_bucket = std::min(least_bucket,back_degree);
            lb_back_degree.erase(v);
	}
    }


    auto begin(unsigned b){
        return(bucket[b].begin());
    }

    auto end(unsigned b){
        return(bucket[b].end());
    }

    
    // erases v from its bucket, updates least_bucket
    void erase(Vertex v, int print_details=0){
        if (vrtx_to_bucket.count(v)){
            bucket[vrtx_to_bucket[v]].erase(v);
	    vrtx_to_bucket.erase(v);
	    update_least_bucket(least_bucket);
        }
    }

    // updates least bucket if nothing is known
    void update_least_bucket(){
        unsigned least = 0;
        while (least < bucket.size() && bucket[least].empty())
            ++least;
        least_bucket = least;
    }

    // updates least bucket if known that the least non-empty bucket
    // is at least lb.
    void update_least_bucket(unsigned lb){
        while (lb < bucket.size() && bucket[lb].empty())
            ++lb;
        least_bucket = lb;
    }

    Vertex get_least_vertex(){
        return(*bucket[least_bucket].begin());
    }

    Vertex get_least_random_vertex(){
	unsigned random = (unsigned) rand() % bucket[least_bucket].size();
	auto it = bucket[least_bucket].begin();
	for (unsigned i = 0; i < random; it++,i++ )
	    ;
        return(*it);
    }
    
    bool need_resize(int print_details){
	if (print_details > 1)
	    std::cout << "Need resize.\n";
        return (least_bucket >= bucket.size()-1);
    }

    bool known_in_last(Vertex v, int print_details=0){
	if (lb_back_degree.count(v) && lb_back_degree[v]>0 && (lb_back_degree[v] > bucket.size()-1)){
	    --lb_back_degree[v];
	    return true;
	}
	else
	    return false;
    }

    
    void print(Graph& g){
        for (size_t i=0; i<bucket.size();++i){
            std::cout << "Bucket: " << i << ": ";
	    for (Vertex w : bucket[i])
		std::cout << g.vertex_to_id(w) << " ";
	    std::cout << std::endl;
        }
	std::cout << "\nFirst non-empty bucket: " << least_bucket << std::endl;
    };

    
    unsigned resize(int print_details=0){
	if (print_details > 1)
	    std::cout << "Resizing to size " << bucket.size() << "." << std::endl;
        bucket.resize(2*bucket.size());
	return(least_bucket);
    };
    
    
private:
   
    std::vector<std::set<Vertex> > bucket;
    
    // for vertices with large back degree: how much bigger is the
    // back degree than bucket.size()-1 (the last bucket). The back
    // degrees for those vertices are not recomputed if this number is positiv.
    std::unordered_map<Vertex,size_t> lb_back_degree;
    
    // the least non-empty bucket. Always exists because the last
    // vertex is not deleted from the buckets 
    size_t least_bucket;
    std::unordered_map<Vertex,size_t> vrtx_to_bucket;
};




#endif
