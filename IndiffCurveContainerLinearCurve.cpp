#include "IndiffCurveContainerLinearCurve.h"
#include "LinearIndiffCurve.h"

IndiffCurveContainerLinearCurve::IndiffCurveContainerLinearCurve(int nbOdPairs) : 
							IndiffCurveContainer(nbOdPairs) {

};

IndiffCurveContainerLinearCurve::~IndiffCurveContainerLinearCurve() {

};

IndiffCurve* IndiffCurveContainerLinearCurve::createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime) {
	return new LinearIndiffCurve(minTime, maxTime, maxToll);
};
