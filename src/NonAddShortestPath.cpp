#include "NonAddShortestPath.h"
#include "ODMatrix.h"
#include "OneSourceBiObjShPath.h"
#include "Point2PointBiObjShPath.h"
#include "Error.h"
#include "Timer.h"
#include "NonAdditivePC.h"
#include "Origin.h"
#include "PairOD.h"

#include <limits>
#include <stdlib.h>
#include <sstream>
#include <cassert>

NonAddShortestPath::NonAddShortestPath(NonAdditivePC *pathCost, int nbNodes, 
					OneSourceBiObjShPath* oneSorceSP, 
					Point2PointBiObjShPath* point2pointSP, ODMatrix* mat) : 
					
					costs_(nbNodes, std::numeric_limits<FPType>::infinity()), 
					oneSorceSP_(oneSorceSP),
					curDestIndex_(-1),
					minCostLabel_(NULL),
					pathCost_(pathCost), 
					point2pointSP_(point2pointSP), 
					mat_(mat)
					 {
	assert(pathCost != NULL && oneSorceSP != NULL && mat != NULL);
	currLink_ = path_.begin();
};

NonAddShortestPath::~NonAddShortestPath(){
};

void NonAddShortestPath::calculate(int originIndex){
	curDestIndex_ = -1;
	oneSorceSP_->calculate(originIndex);
	for (OriginIterator it = mat_->begin(); it != mat_->end(); ++it){
		Origin* origin = *it;
		if (origin->getIndex() == originIndex) {
			for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
				PairOD* dest = *jt;
				int destIndex = dest->getIndex();
				std::pair<FPType, BiObjLabel*> costAndLabel = calculateCostToDest(destIndex, 
					dest->getODIndex(), oneSorceSP_->getLabels());
				costs_[destIndex] = costAndLabel.first;
				doSomethingWithMinCostLabel(costAndLabel.second);
			}
			break;
		}
	}
};

void NonAddShortestPath::doSomethingWithMinCostLabel(BiObjLabel* minCostLabel) {

};

std::pair<FPType, BiObjLabel*> NonAddShortestPath::calculateCostToDest(int destIndex, 
				int odPairIndex, const BiObjLabelContainer& labels){
	FPType minCost = std::numeric_limits<FPType>::infinity();
	BiObjLabel* labelWithMinCost = NULL;
	for (LabelsIterator labelIt = labels.begin(destIndex); labelIt != labels.end(destIndex); ++labelIt){
		BiObjLabel* label = *labelIt;
		FPType cost = pathCost_->calculate(label->getTime(), label->getToll(), odPairIndex);
		if (cost < minCost) {
			minCost = cost;
			labelWithMinCost = label;
		}
	}
	assert(labelWithMinCost != NULL && minCost != std::numeric_limits<FPType>::infinity());
	return std::make_pair<FPType, BiObjLabel*>(minCost, labelWithMinCost);
};

void NonAddShortestPath::calculate(int originIndex, int destIndex, int odPairIndex){
	assert(originIndex >= 0 && destIndex >= 0);
	curDestIndex_ = destIndex;

	point2pointSP_->calculate(originIndex, destIndex, odPairIndex);
	
	std::pair<FPType, BiObjLabel*> costAndLabel = calculateCostToDest(destIndex, odPairIndex, 
			point2pointSP_->getLabels());

	path_.clear();
		
	minCostLabel_ = costAndLabel.second;
	
	costs_[destIndex] = costAndLabel.first;

};

FPType NonAddShortestPath::getCost(int destIndex) const{
	return costs_[destIndex];
};

StarLink* NonAddShortestPath::getInComeLink(int destIndex) const{
	assert(destIndex >= 0 && curDestIndex_ != -1);
	
	if (path_.empty()) {
		assert(minCostLabel_ != NULL);
		createPath();
		currLink_ = path_.begin();
		assert(currLink_ != path_.end());
	}

	if (currLink_ == path_.end()) {
		return NULL;
	}
	StarLink* link = *currLink_;
	if (link->getNodeToIndex() == destIndex) {
		++currLink_;
		return link;
	} 
	throw Error("Should never end up here. Non-additive shortest path was called with path that was not created.");
	
	return NULL;
};

void NonAddShortestPath::createPath() const {
	point2pointSP_->createPath(minCostLabel_, path_);
}

