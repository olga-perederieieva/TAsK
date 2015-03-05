#include "AONUsual.h"
#include "InitialiseSolution.h"
#include "ShortestPath.h"
#include "Origin.h"

AONUsual::AONUsual(const ODMatrix &mat, ShortestPath* shPath) : AONAssignment(mat, shPath) {
};

AONUsual::~AONUsual(){

};

FPType AONUsual::calculateTravelTime(const PairOD& dest) const {
	return dest.getDemand() * shPath_->getCost(dest.getIndex());
};

FPType AONUsual::performAssignmentForOrigin(InitialiseSolution *solution, Origin* origin) {
	int originIndex = origin->getIndex();
	shPath_->calculate(originIndex);
	FPType minTravelTime = 0.0;
	for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
		PairOD* dest = *jt;
		solution->initialiseSolution(dest, shPath_, originIndex);
		minTravelTime += calculateTravelTime(*dest);
	}
	return minTravelTime;
};