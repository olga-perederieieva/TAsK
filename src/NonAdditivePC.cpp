#include "NonAdditivePC.h"
#include "StarLink.h"
#include "Path.h"

#include <cassert>

NonAdditivePC::NonAdditivePC(const IndiffCurveContainer& curves, const TollContainerType& tolls) : 
						curves_(curves), tolls_(tolls) {

};

NonAdditivePC::~NonAdditivePC(){

};

FPType NonAdditivePC::calculate(Path *path, int odIndex) const{
	assert(path != NULL && odIndex >=0);
	TollType totalToll = 0.0;
	FPType totalTime = 0.0;
	for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
		StarLink* link = *it;
		totalTime += link->getTime();
		totalToll += tolls_[link->getIndex()];
	}
	return calculate(totalTime, totalToll, odIndex);
};

FPType NonAdditivePC::calculate(FPType totalTime, TollType totalToll, int odIndex) const{
	return totalTime + evaluateCurve(totalToll, odIndex);
};

FPType NonAdditivePC::evaluateCurve(TollType totalToll, int odIndex) const {
	return (curves_.getCurve(odIndex))->evaluate(totalToll);
};
