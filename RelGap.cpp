#include "RelGap.h"
#include "Error.h"
#include "Origin.h"
#include "PairOD.h"
#include "ODMatrix.h"
#include "ShortestPath.h"
#include "StarNetwork.h"

#include <stdlib.h>
#include <iostream>
#include <math.h>

RelGap::RelGap(FPType precision, StarNetwork *net, ODMatrix *mat, ShortestPath *shPath) : ConvMeasure(precision),
										net_(net), mat_(mat), shPath_(shPath) {

};

RelGap::~RelGap(){

};

FPType RelGap::getMinTravelTime(){
	FPType minTravelTime = 0.0;
	for (OriginIterator it = mat_->begin(); it != mat_->end(); ++it){
		Origin* origin = *it;
		shPath_->calculate(origin->getIndex());
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
			PairOD* dest = *jt;
			minTravelTime += dest->getDemand() * shPath_->getCost(dest->getIndex()); 
		}
	}
	return minTravelTime;
};

FPType RelGap::getTotalTravelTime() {
	FPType totalTravelTime = 0.0;
	for (StarLink *link = net_->beginOnlyLink(); link != NULL; link = net_->getNextOnlyLink()) {
		totalTravelTime += link->getFlow() * link->getTime();
		if (totalTravelTime != totalTravelTime) {
			std::cout << "totalTravelTime = " << totalTravelTime << " flow = " << 
				link->getFlow() << " time = " << link->getTime() << std::endl;
			throw Error("totalTravelTime is NaN. Something went horribly wrong.");
		}
	}
	return totalTravelTime;
};

FPType RelGap::calculateGapValue(FPType minTravelTime, FPType totalTravelTime) {
	if (fabs(totalTravelTime) <= 1e-25) {
		return std::numeric_limits<FPType>::infinity( );
	}
	FPType gap = 1.0 - minTravelTime / totalTravelTime;
	return gap;
};


FPType RelGap::calculateGap(){
	return calculateGapValue(getMinTravelTime(), getTotalTravelTime());
};

FPType RelGap::calculateGap(FPType minTravelTime) {
	return calculateGapValue(minTravelTime, getTotalTravelTime());
};

