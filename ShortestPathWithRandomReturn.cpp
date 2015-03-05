#include "ShortestPathWithRandomReturn.h"
#include "Utils.h"

#include <time.h>
#include <stdlib.h>
#include <limits>
#include <cassert>

ShortestPathWithRandomReturn::ShortestPathWithRandomReturn(ShortestPath* shPath) : 
								shPath_(shPath), returnInf_(false), nbIters_(1) {
	srand(time(NULL));
};

ShortestPathWithRandomReturn::~ShortestPathWithRandomReturn() {
};

void ShortestPathWithRandomReturn::calculate(int originIndex) {
	returnInf_ = false;
	shPath_->calculate(originIndex);
	if (originIndex == 0) ++nbIters_;
}; 

FPType ShortestPathWithRandomReturn::getProbability() const {
	return 0.1 + 1.0/nbIters_;
}; 

void ShortestPathWithRandomReturn::calculate(int originIndex, int destIndex, int odPairIndex) {
	if (Utils::generateRndNumber(1.0) < getProbability()) {
		shPath_->calculate(originIndex, destIndex, odPairIndex);
		returnInf_ = false;
	} else {
		returnInf_ = true;
	}
};

FPType ShortestPathWithRandomReturn::getCost(int destIndex) const {
	if (returnInf_) return std::numeric_limits<FPType>::infinity();
	return shPath_->getCost(destIndex);
};

StarLink* ShortestPathWithRandomReturn::getInComeLink(int destIndex) const {
	assert(returnInf_ == false);
	return shPath_->getInComeLink(destIndex);
};