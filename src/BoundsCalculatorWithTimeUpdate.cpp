#include "BoundsCalculatorWithTimeUpdate.h"

#include <cassert>

BoundsCalculatorWithTimeUpdate::BoundsCalculatorWithTimeUpdate(StarNetwork& net,
			const TollContainerType& tolls) : 
			BoundsCalculatorForBSP(net, tolls), destNode_(-1), prevODIndex_(-1) {
	
};

BoundsCalculatorWithTimeUpdate::~BoundsCalculatorWithTimeUpdate() {
};

void BoundsCalculatorWithTimeUpdate::updateTimeBounds(int destNodeIndex, int odPairIndex) {
	
	if (odPairIndex != prevODIndex_) {
		destNode_ = destNodeIndex;
		prevODIndex_ = odPairIndex;
		spp_->calculate(destNode_);
	}
};

FPType BoundsCalculatorWithTimeUpdate::getTimeLowerBound(int nodeIndex, int destNodeIndex) {
	assert(destNode_ == destNodeIndex);
	return spp_->getCost(nodeIndex); 
};

StarLink* BoundsCalculatorWithTimeUpdate::getInComeLink(int destIndex) const {
	return spp_->getInComeLink(destIndex);
};