#include "NonAddShortestPathForAON.h"
#include "UtilsForDomination.h"
#include "Error.h"
#include "BiObjLabel.h"

#include <cassert>

NonAddShortestPathForAON::NonAddShortestPathForAON(
						NonAdditivePC *pathCost, int nbNodes, OneSourceBiObjShPath* oneSorceSP, 
						ODMatrix* mat) : NonAddShortestPath(pathCost, nbNodes, oneSorceSP, NULL,
						mat), minCostLabels_(nbNodes), 
						shoulResetPath_(true) {
	for (int i = 0; i < nbNodes; ++i) {
		minCostLabels_[i] = NULL;
	}
	linkIt_ = currPath_.begin();
};

NonAddShortestPathForAON::~NonAddShortestPathForAON() {

};

StarLink* NonAddShortestPathForAON::getInComeLink(int destIndex) const {
	if (shoulResetPath_) {
		currPath_.clear();
		assert(minCostLabels_[destIndex] != NULL);
		UtilsForDomination::createPathFromLabel(minCostLabels_[destIndex], currPath_);
		shoulResetPath_ = false;
		linkIt_ = currPath_.begin();
		assert(linkIt_ != currPath_.end());
	}
	if (linkIt_ == currPath_.end()) {
		shoulResetPath_ = true;
		return NULL;
	}
	StarLink* link = *linkIt_;
	if (link->getNodeToIndex() == destIndex) {
		++linkIt_;
		return link;
	}
	throw Error("Should never end up here. Non-additive for AON shortest path was called with path that was not created.");
	return NULL; 
};

void NonAddShortestPathForAON::doSomethingWithMinCostLabel(BiObjLabel* minCostLabel) {
	minCostLabels_[minCostLabel->getOwner()] = minCostLabel;
};