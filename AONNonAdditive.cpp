#include "AONNonAdditive.h"
#include "NonAddShortestPathForAON.h"
#include "InitialiseSolution.h"

AONNonAdditive::AONNonAdditive(const ODMatrix &mat, NonAddShortestPathForAON* shPath) : 
				AONUsual(mat, shPath){

};

AONNonAdditive::~AONNonAdditive(){
	delete shPath_;
	shPath_ = NULL;
};

FPType AONNonAdditive::calculateTravelTime(const PairOD& dest) const{
	return 0.0;
};
		