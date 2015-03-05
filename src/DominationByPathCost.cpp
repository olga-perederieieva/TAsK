#include "DominationByPathCost.h"
#include "NonAdditivePC.h"
#include "BiObjLabelContainer.h"

#include <limits>

DominationByPathCost::DominationByPathCost(const NonAdditivePC& pathCost) : 
			pathCost_(pathCost), bestKnownPathCost_(std::numeric_limits<FPType>::infinity()) {

};

DominationByPathCost::~DominationByPathCost() {

};

void DominationByPathCost::addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
			BiObjLabel* label, int destIndex, int odIndex) {
	if (nodeIndex == destIndex) {
		FPType pathCost = pathCost_.calculate(label->getTime(), label->getToll(), odIndex);
		if (pathCost < bestKnownPathCost_) {
			bestKnownPathCost_ = pathCost;
		} else {
			return; // if new label has path cost larger than best know, it is not added to labels
		}
	}

	labels.pushBackLabel(nodeIndex, label);
};

void DominationByPathCost::resetBestKnownPathCost() {
	bestKnownPathCost_ = std::numeric_limits<FPType>::infinity();
};

bool DominationByPathCost::isWorth(const BiObjLabelContainer& labels, int destIndex, 
			FPType timeLowerBound, TollType tollLowerBound, int odIndex) const {

	FPType pathCostLowerBound = pathCost_.calculate(timeLowerBound, tollLowerBound, odIndex);
	if (pathCostLowerBound >= bestKnownPathCost_) return false;	
	return true;
};

void DominationByPathCost::updateBestKnown(const BiObjLabelContainer& labels, int destIndex, 
				int odIndex) {
	for (LabelsIterator it = labels.begin(destIndex); it != labels.end(destIndex); ++it) {
		FPType pathCost = pathCost_.calculate((*it)->getTime(), (*it)->getToll(), odIndex);
		if (pathCost < bestKnownPathCost_) {
			bestKnownPathCost_ = pathCost;
		}
	}
};
