#include "AONBushBased.h"
#include "Origin.h"
#include "ShortestPath.h"
#include "InitialiseSolution.h"

#include <limits>

AONBushBased::AONBushBased(const ODMatrix &mat, ShortestPath* shPath, int nbNodes) : 
					AONAssignment(mat, shPath), nbNodes_(nbNodes) {

};

AONBushBased::~AONBushBased(){

};

FPType AONBushBased::performAssignmentForOrigin(InitialiseSolution *solution, Origin* origin){
	int originIndex = origin->getIndex();
	shPath_->calculate(originIndex);
	int nodes[nbNodes_];
	for (int i = 0; i < nbNodes_; ++i){
		nodes[i] = -1;
	}
	nodes[originIndex] = 1; 
	for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
		PairOD* dest = *jt;
		solution->initialiseSolution(dest, shPath_, originIndex);
		nodes[dest->getIndex()] = 1; 
	}
	// iterate other nodes that are not destinations
	FPType dist = 0.0;
	for (int i = 0; i < nbNodes_; ++i){
		if (nodes[i] == -1){
			
			dist = shPath_->getCost(i);
			if (dist != std::numeric_limits<FPType>::infinity()) { // otherwise it is another origin node
				PairOD fakeDest(i, 0.0);
				solution->initialiseSolution(&fakeDest, shPath_, originIndex);
				nodes[i] = 1;
			}
		}
	}
	return 0.0;
};