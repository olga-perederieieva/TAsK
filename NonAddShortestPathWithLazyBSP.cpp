#include "NonAddShortestPathWithLazyBSP.h"
#include "UtilsForDomination.h"
#include "OneSourceBiObjShPath.h"

NonAddShortestPathWithLazyBSP::NonAddShortestPathWithLazyBSP(
			NonAdditivePC *pathCost, int nbNodes, OneSourceBiObjShPath* oneSorceSP, 
			ODMatrix* mat) : NonAddShortestPath(pathCost, nbNodes, oneSorceSP, 
			NULL, mat), prevOriginIndex_(-1) {
	std::cout << "NonAddShortestPathWithLazyBSP*******************" << std::endl;
};

NonAddShortestPathWithLazyBSP::~NonAddShortestPathWithLazyBSP() {

};

void NonAddShortestPathWithLazyBSP::calculate(int originIndex, int destIndex, int odPairIndex) {
	curDestIndex_ = destIndex;
	if (originIndex == prevOriginIndex_) {
		updateLabelTravelTimes(oneSorceSP_->getLabels());
	} else {
		oneSorceSP_->calculate(originIndex);
		prevOriginIndex_ = originIndex;
	}
	std::pair<FPType, BiObjLabel*> costAndLabel = calculateCostToDest(destIndex, odPairIndex, 
			oneSorceSP_->getLabels());

	path_.clear();	
	minCostLabel_ = costAndLabel.second;
	costs_[destIndex] = costAndLabel.first;
};

void NonAddShortestPathWithLazyBSP::updateLabelTravelTimes(const BiObjLabelContainer& labels) {
	for (LabelsIterator labelIt = labels.begin(curDestIndex_); labelIt != labels.end(curDestIndex_); ++labelIt){
		BiObjLabel* label = *labelIt;
		label->setTime(recalculatePathTime(label));
	}
};

FPType NonAddShortestPathWithLazyBSP::recalculatePathTime(BiObjLabel* destLabel) const {
	BiObjLabel* label = destLabel;
	FPType travelTime = 0;
	while (label != NULL) {
		if (label->getPrevLink() != NULL) travelTime += label->getPrevLink()->getTime();
		label = label->getPrevLabel();
	}
	return travelTime;
};

void NonAddShortestPathWithLazyBSP::createPath() const {
	UtilsForDomination::createPathFromLabel(minCostLabel_, path_);
};