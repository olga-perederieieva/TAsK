#include "PathBasedFlowMove.h"
#include "DescDirectionPath.h"

#include <stdlib.h>

PathBasedFlowMove::PathBasedFlowMove(DescDirectionPath* pathDirectionAlgo) : pathDirection_(NULL),
									 size_(0), 
								     currDirection_(0),
								     pathDirectionAlgo_(pathDirectionAlgo) {

};

bool PathBasedFlowMove::calculatePathDirection(const std::list<Path*> &paths){
	bool isEquilibrated = false;
	pathDirection_ = pathDirectionAlgo_->calculatePathDirection(size_, paths, isEquilibrated);
	return isEquilibrated;
};

PathAndDirection* PathBasedFlowMove::beginPathDirection() const {
	currDirection_ = 0;
	if (size_ == 0) return NULL;
	return pathDirection_[0];
};

PathAndDirection* PathBasedFlowMove::getNextPathDirection() const {
	++currDirection_;
	if (currDirection_ >= size_) return NULL; 
	return  pathDirection_[currDirection_];
};
