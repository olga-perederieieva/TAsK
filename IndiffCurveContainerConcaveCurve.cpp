#include "IndiffCurveContainerConcaveCurve.h"
#include "ConcaveIndiffCurve.h"

IndiffCurveContainerConcaveCurve::IndiffCurveContainerConcaveCurve(int nbOdPairs) : 
				IndiffCurveContainer(nbOdPairs) {

};

IndiffCurveContainerConcaveCurve::~IndiffCurveContainerConcaveCurve() {

};

IndiffCurve* IndiffCurveContainerConcaveCurve::createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime) {
	return new ConcaveIndiffCurve(minTime, maxTime, maxToll);
};