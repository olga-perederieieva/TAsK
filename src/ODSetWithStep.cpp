#include "ODSetWithStep.h"
#include "Utils.h"
#include "PathBasedFlowMoveWithStep.h"
#include "PathAndDirection.h"
#include "StarNetwork.h"
#include "Path.h"
#include "StarLink.h"

#include <set>
#include <vector>

PathBasedFlowMoveWithStep* ODSetWithStep::flowMoveStep_ = NULL;
FPType* ODSetWithStep::linkFlows_ = NULL;
bool ODSetWithStep::wasInitialised_ = false;
FPType *ODSetWithStep::projDir_ = NULL;
int * ODSetWithStep::indexes_ = NULL;
int ODSetWithStep::size_ = 0;
int ODSetWithStep::nbLinks_ = 0;

ODSetWithStep::ODSetWithStep(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net,
			ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMoveWithStep* flowMove) : 
			ODSet(index, destIndex, originIndex, pathCost, net, shPath, zeroFlow, flowMove) { 
	flowMoveStep_ = flowMove;
	nbLinks_ = net->getNbLinks();
	if (!wasInitialised_) {
			linkFlows_ = new FPType[nbLinks_];
			projDir_ = new FPType[nbLinks_];
			indexes_ = new int[nbLinks_];
			for (int i = 0; i < nbLinks_; ++i) {
				linkFlows_[i] = 0.0;
				projDir_[i] = 0.0;
				indexes_[i] = -1;
			}
	}
	wasInitialised_ = true;
}; 

ODSetWithStep::~ODSetWithStep() {
	delete[] linkFlows_;
	delete[] projDir_;
	delete[] indexes_;
	linkFlows_ = NULL;
	projDir_ = NULL;
	indexes_ = NULL;
}; 

FPType* ODSetWithStep::getProjectedDirection() {
	 return projDir_;
}; 

FPType ODSetWithStep::getUpperBound() {
	FPType ub = std::numeric_limits<FPType>::infinity();
	FPType dir = 0.0;
	FPType ubTmp = 0.0;
	for (PathAndDirection *pathDir = flowMoveStep_->beginPathDirection(); pathDir != NULL; 
		pathDir = flowMoveStep_->getNextPathDirection()) {
		dir = pathDir->getDirectionValue();
		if (dir < 0.0) {
			ubTmp = -(pathDir->getPath()->getFlow()) / dir;
			if (ubTmp < ub) {
				ub = ubTmp;
			}	
		}
	}
	return ub;
}; 

FPType* ODSetWithStep::getLinkFlows() {
	 return linkFlows_;
}; 

int ODSetWithStep::getSize(){
	return size_;
};

int* ODSetWithStep::getIndexes(){
	return indexes_;
};

void ODSetWithStep::calculateProjectedDirection() {
	// assign zero
	for (int i = 0; i < nbLinks_; ++i) {
		projDir_[i] = 0.0;
	}
	
	// project path-based direction on links
	Path *path = NULL;
	int linkIndex = -1;

	for (PathAndDirection *pathDir = flowMoveStep_->beginPathDirection(); pathDir != NULL; pathDir = flowMoveStep_->getNextPathDirection()) {
		path = pathDir->getPath();
		
		for(StarLinkIterator it = path->begin(); it != path->end(); ++it){
			StarLink* link = *it;
			linkIndex = link->getIndex();
			projDir_[linkIndex] += pathDir->getDirectionValue();
			linkFlows_[linkIndex] = link->getFlow();
		}
	}
	
	int count = 0;
	for (int i = 0; i < nbLinks_; ++i) {
		if (projDir_[i] != 0.0) {
			indexes_[count] = i;
			++count;
		}
	}

	size_ = count;
	flowMoveStep_->setDescDirection(this);
}; 

int ODSetWithStep::getOdIndex() const {
	return getIndex();
};

PathBasedFlowMove* ODSetWithStep::getPathBasedFlowMove() const {
	return flowMoveStep_;
};