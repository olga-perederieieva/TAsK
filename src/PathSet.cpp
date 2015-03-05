#include "PathSet.h"
#include "ODSetWithStep.h"
#include "ODSetGPApp3.h"
#include "Error.h"
#include "Origin.h"
#include "Path.h"
#include "ODMatrix.h"
#include "AONAssignment.h"

#include <cassert>
#include <sstream>

PathSet::PathSet(ODSetType setType, StarNetwork *net, ODMatrix *mat, ShortestPath *shPath,
			PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMove* flowMove,  
			PathBasedFlowMoveWithStep* flowMoveWithStep, PathBasedFlowMoveGP* flowMoveGP,
			AONAssignment* aon) : 
			odSetList_(mat->getNbODPairs()),
			size_(mat->getNbODPairs()), currPath_(NULL),
			aon_(aon){

	int odIndex = -1;
	for (OriginIterator it = mat->begin(); it != mat->end(); ++it){
		Origin* origin = *it;
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
			PairOD* dest = *jt;
			odIndex = dest->getODIndex();
			if (setType == PEAPP3) {
				odSetList_[odIndex] = new ODSet(odIndex, dest->getIndex(), origin->getIndex(),
												pathCost, net, shPath, 
					zeroFlow, flowMove);
			} else if (setType == WITH_STEP) {
				odSetList_[odIndex] = new ODSetWithStep(odIndex, dest->getIndex(), origin->getIndex(),
														pathCost, net, shPath, zeroFlow,
														flowMoveWithStep);
			} else if (setType == GPAPP3) {
				odSetList_[odIndex] = new ODSetGPApp3(odIndex, dest->getIndex(), origin->getIndex(),
													  pathCost, net, shPath, zeroFlow,
													  flowMoveGP, mat);
			} else {
				throw Error("Unexpected OD set type.");
			}
		}
	}
};

PathSet* PathSet::createSetPEAPP3(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, 
			PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMove* flowMove, AONAssignment* aon){
	return new PathSet(PEAPP3, net, mat, shPath, pathCost, zeroFlow, flowMove, NULL, NULL, aon);
};

PathSet* PathSet::createSetWithStep(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, 
			PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMoveWithStep* flowMove, AONAssignment* aon){
	return new PathSet(WITH_STEP, net, mat, shPath, pathCost, zeroFlow, NULL, flowMove, NULL, aon);
};
PathSet* PathSet::createSetGPAPP3(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath,
			PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMoveGP* flowMove, AONAssignment* aon){
	return new PathSet(GPAPP3, net, mat, shPath, pathCost, zeroFlow, NULL, NULL, flowMove, aon);
};

PathSet::~PathSet(){
	for (int i = 0; i < size_; ++i) {
		delete odSetList_[i];
	}
	delete aon_;
};

void PathSet::initialise(){
	aon_->execute(this);
	for (ConstODSetIterator odSetIt = begin(); odSetIt != end(); ++odSetIt){
		(*odSetIt)->projectPathFlowOnLinks();
	}
};

void PathSet::initialiseItself(StarLink* link, PairOD *dest, int originIndex){
	
	int odIndex = dest->getODIndex();
	// check if a new path has to be created
	if (link->getNodeToIndex() == (odSetList_[odIndex])->getDestIndex()) { 
		currPath_ = new Path();
		currPath_->setFlow(dest->getDemand());
		(odSetList_[odIndex])->addPath(currPath_);
	}
	currPath_->addLinkToPath(link);
	
};


ConstODSetIterator PathSet::begin() const{
	return odSetList_.begin();
};

ConstODSetIterator PathSet::end() const {
	return odSetList_.end();
};

void PathSet::print(){
	for (ConstODSetIterator odSetIt = begin(); odSetIt != end(); ++odSetIt) {
		(*odSetIt)->print();
	}
};

ODSet* PathSet::getODSetByIndex(int odIndex) const{
	assert(odIndex >= 0 && odIndex < size_);
	return odSetList_[odIndex];
};
