#include "LazyShortestPath.h"

LazyShortestPath::LazyShortestPath(ShortestPath* shPath) : shPath_(shPath), 
					prevOriginIndex_(-1), originChanged_(true), newPathCost_(0){

};

LazyShortestPath::~LazyShortestPath() {
	delete shPath_;
};

void LazyShortestPath::calculate(int originIndex) {
	shPath_->calculate(originIndex);
	originChanged_ = true;
};  

void LazyShortestPath::calculate(int originIndex, int destIndex, int odPairIndex) {
	if (originIndex == prevOriginIndex_) {
		updatePathCost(destIndex);
		originChanged_ = false;
	} else {
		shPath_->calculate(originIndex);
		prevOriginIndex_ = originIndex;
		originChanged_ = true;
	}
}; 
		
FPType LazyShortestPath::getCost(int destIndex) const {
	if (originChanged_) {
		return shPath_->getCost(destIndex);
	} else {
		return newPathCost_;
	}
};
		
StarLink* LazyShortestPath::getInComeLink(int destIndex) const {
	return shPath_->getInComeLink(destIndex);
}; 

void LazyShortestPath::updatePathCost(int destIndex) {
	StarLink *link = shPath_->getInComeLink(destIndex);
	FPType pathCost = 0;
	while (link != NULL) {
		pathCost += link->getTime();
		link = shPath_->getInComeLink(link->getNodeFromIndex());
	}
	newPathCost_ = pathCost;
};