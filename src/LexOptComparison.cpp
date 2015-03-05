#include "LexOptComparison.h"
#include "BiObjLabel.h"

bool LexOptComparison::operator() (BiObjLabel* lhs, BiObjLabel* rhs) const {
  	// for label-setting labels MUST be sorted lexicographically. Otherwise, the algorithm won't
  	// converge. In fact, we need to have a guarantee that the label taken out of the list is
  	// non-dominated (this is guaranteed if the label is lexicographically minimal)
   	return  (lhs->getTime() < rhs->getTime() ||
   			(lhs->getTime() == rhs->getTime() && lhs->getToll() < rhs->getToll()) ); 
   	// return (lhs->getTime() + lhs->getToll() < rhs->getTime() +  rhs->getToll()); // sum does not work
   	// in our case. According to the paper, costs must be strictly positive for correctedness of the algorithm
   	// which is not the case in our problem
};