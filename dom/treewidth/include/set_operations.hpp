#ifndef SET_OPERATIONS_INCLUDED
#define SET_OPERATIONS_INCLUDED

#include <algorithm> // std::includes

template <class T> // T must be a class having input iterators
class SetOperations{
    typedef typename T::iterator Vertex_it;

public:
    // compute the size of std::set_difference(...)
    // without computing set_difference
    // use code from http://en.cppreference.com/w/cpp/algorithm/set_difference
    static size_t size_of_difference (Vertex_it first1, Vertex_it last1,
				    Vertex_it first2, Vertex_it last2);
    // input: iterators to two sets S1 and S2
    // output: 0  if S1 and S2 are incomparable w.r.t. the subset relation
    //         1  if S1 \subseteq  S2
    //         -1 if S2 \subsetneq S1
    static int subset (Vertex_it first1, Vertex_it last1,
                       Vertex_it first2, Vertex_it last2);    

};

template <typename T>
size_t SetOperations<T>::size_of_difference (Vertex_it first1, Vertex_it last1,
					    Vertex_it first2, Vertex_it last2){
	size_t num=0;
	while (first1 != last1) {
	    if (first2 == last2)
		return(num + std::distance(first1, last1));
	    if (*first1 < *first2) {
		++num;
		++first1;
	    } else {
		if (! (*first2 < *first1)) {
		    ++first1;
		}
		++first2;
	    }
	}
	return num;
    }

template <typename T>
int SetOperations<T>::subset (Vertex_it first1, Vertex_it last1,
                              Vertex_it first2, Vertex_it last2){
    while ((first1 != last1) && (first2 != last2)) {
        if (*first1 < *first2){// S1 not a subset of S2
            if (std::includes(++first1,last1,first2,last2)){
                return (-1);// S2 \subsetneq S1
            }
            else
                return (0);// incomparable
        }
        if (*first2 < *first1){// S2 not a subset of S1
            if (std::includes(++first2,last2,first1,last1)){
                return (1);// S1 \subseteq S2
            }
            else
                return (0);// incomparable
        }
        // move first1 and first2 to new values or to the end of the
        // region
        // this is needed for vectors, but not for (ordered) sets
        auto pre_first1 = first1++;
        while (first1 != last1 && (*first1 == *pre_first1))
            ++first1;
        auto pre_first2 = first2++;
        while (first2 != last2 && (*first2 == *pre_first2))
            ++first2;

    }
    if (first1 == last1)
        return(1); // S1 \subseteq   S2
    if (first2 == last2)
        return(-1);// S2 \subsetneq  S1
    return(0);// incomparable
}


#endif
