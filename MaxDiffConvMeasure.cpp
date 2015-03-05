#include "MaxDiffConvMeasure.h"
#include "PathSet.h"
#include "ShortestPath.h"
#include "Origin.h"
#include "Path.h"
#include "ODSet.h"
#include "ODMatrix.h"

#include <cassert>
#include <limits>

MaxDiffConvMeasure::MaxDiffConvMeasure(FPType precision, ShortestPath* shPath,
		ODMatrix* mat, PathSet* pathSet) : 
		ConvMeasure(precision), shPath_(shPath), mat_(mat), pathSet_(pathSet) {
	assert(shPath != NULL);
};

MaxDiffConvMeasure::~MaxDiffConvMeasure(){

};

FPType MaxDiffConvMeasure::calculateMaxPathCost(int odIndex){
	ODSet* set = pathSet_->getODSetByIndex(odIndex);
	set->updateSet(); 
 	FPType maxCost = 0.0;
	for (PathIterator pathIt = set->begin(); pathIt != set->end(); ++pathIt) {
		FPType cost = (*pathIt)->getCurrCost();
		if (cost > maxCost) maxCost = cost;
	}
	return maxCost;
};

FPType MaxDiffConvMeasure::calculateGap(){
	FPType maxDiff = 0.0;
	for (OriginIterator it = mat_->begin(); it != mat_->end(); ++it){
		Origin* origin = *it;
		shPath_->calculate(origin->getIndex());
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
			PairOD* dest = *jt;
			FPType minPathCost = shPath_->getCost(dest->getIndex());
			assert(minPathCost != std::numeric_limits<FPType>::infinity());
			FPType diff = calculateMaxPathCost(dest->getODIndex()) - minPathCost;
			if (diff > maxDiff) {
				maxDiff = diff;
			}
		}
	}

	return maxDiff;
};
