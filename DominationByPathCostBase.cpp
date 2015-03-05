#include "DominationByPathCostBase.h"
#include "BiObjLabelContainer.h"
#include "UtilsForDomination.h"

DominationByPathCostBase::DominationByPathCostBase() {

};

DominationByPathCostBase::~DominationByPathCostBase() {

};


void DominationByPathCostBase::addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
			BiObjLabel* label, int destIndex, int odIndex) {
	labels.pushBackLabel(nodeIndex, label);
};

void DominationByPathCostBase::resetBestKnownPathCost() {

};

bool DominationByPathCostBase::isWorth(const BiObjLabelContainer& labels, int destIndex,
			FPType timeLowerBound, TollType tollLowerBound, int odIndex) const {
	return !UtilsForDomination::isDominatedByLabelInDestNode(labels, destIndex, 
				timeLowerBound, tollLowerBound);;
};

void DominationByPathCostBase::updateBestKnown(const BiObjLabelContainer& labels, int destIndex, 
			int odIndex) {

};
