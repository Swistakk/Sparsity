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
        bucket_(3)
    {
        for (Vertex v = 0; v<g.num_vertices(); ++v){
            if (g.getDegree(v) > 1){ // at least 2
                bucket_[2].insert(v);
                vrtx_to_bucket_[v] = 2;
            }
            else{
                bucket_[g.getDegree(v)].insert(v); // 0 or 1
                vrtx_to_bucket_[v] = g.getDegree(v);
            }
        }
    
        if (!bucket_[0].empty())
            least_bucket_ = 0;
        else {
            if (!bucket_[1].empty())
                least_bucket_ = 1;
            else
                least_bucket_ = 2;
        }
    }

    Bucket(unsigned size):
        bucket_(size){
        least_bucket_ = size;
    };

    size_t size(){
	return (bucket_.size());
    }
    
    void insert(Vertex v, size_t back_degree){
        if (back_degree >= bucket_.size()-1){
            bucket_[bucket_.size()-1].insert(v);
	    least_bucket_ = std::min(least_bucket_,bucket_.size()-1);
	    lb_back_degree_[v] = back_degree - bucket_.size() + 1;
	    vrtx_to_bucket_[v] = bucket_.size()-1;
	}
        else{
            bucket_[back_degree].insert(v);
	    lb_back_degree_.erase(v);
	    vrtx_to_bucket_[v] = back_degree;
            least_bucket_ = std::min(least_bucket_,back_degree);
            lb_back_degree_.erase(v);
	}
    }


    auto begin(unsigned b){
        return(bucket_[b].begin());
    }

    auto end(unsigned b){
        return(bucket_[b].end());
    }

    
    // erases v from its bucket, updates least_bucket_
    void erase(Vertex v, bool print_details=false){
        if (vrtx_to_bucket_.count(v)){
            bucket_[vrtx_to_bucket_[v]].erase(v);
	    vrtx_to_bucket_.erase(v);
	    update_least_bucket(least_bucket_);
        }
    }

    // updates least bucket if nothing is known
    void update_least_bucket(){
        unsigned least = 0;
        while (least < bucket_.size() && bucket_[least].empty())
            ++least;
        least_bucket_ = least;
    }

    // updates least bucket if known that the least non-empty bucket
    // is at least lb.
    void update_least_bucket(unsigned lb){
        while (lb < bucket_.size() && bucket_[lb].empty())
            ++lb;
        least_bucket_ = lb;
    }

    Vertex get_least_vertex(){
        return(*bucket_[least_bucket_].begin());
    }

    Vertex get_least_random_vertex(){
	unsigned random = (unsigned) rand() % bucket_[least_bucket_].size();
	auto it = bucket_[least_bucket_].begin();
	for (unsigned i = 0; i < random; it++,i++ )
	    ;
        return(*it);
    }
    
    bool need_resize(bool print_details){
	if (print_details)
	    std::cout << "Need resize.\n";
        return (least_bucket_ >= bucket_.size()-1);
    }

    bool known_in_last(Vertex v, bool print_details=false){
	if (lb_back_degree_.count(v) && lb_back_degree_[v]>0 && (lb_back_degree_[v] > bucket_.size()-1)){
	    --lb_back_degree_[v];
	    return true;
	}
	else
	    return false;
    }

    
    void print(Graph& g){
        for (size_t i=0; i<bucket_.size();++i){
            std::cout << "Bucket: " << i << ": ";
	    for (Vertex w : bucket_[i])
		std::cout << g.vertex_to_id(w) << " ";
	    std::cout << std::endl;
        }
	std::cout << "\nFirst non-empty bucket: " << least_bucket_ << std::endl;
    };

    
    unsigned resize(bool print_details){
	if (print_details)
	    std::cout << "Resizing to size " << 2*bucket_.size() << "." << std::endl;
        bucket_.resize(2*bucket_.size());
	return(least_bucket_);
    };
    
    
private:
   
    std::vector<std::set<Vertex> > bucket_;
    
    // for vertices with large back degree: how much bigger is the
    // back degree than bucket_.size()-1 (the last bucket). The back
    // degrees for those vertices are not recomputed if this number is positiv.
    std::unordered_map<Vertex,size_t> lb_back_degree_;
    
    // the least non-empty bucket. Always exists because the last
    // vertex is not deleted from the buckets 
    size_t least_bucket_;
    std::unordered_map<Vertex,size_t> vrtx_to_bucket_;
};




#endif
