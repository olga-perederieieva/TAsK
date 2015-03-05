#include "IndiffCurveContainerConvexCurve.h"
#include "ConvexIndiffCurve.h"

IndiffCurveContainerConvexCurve::IndiffCurveContainerConvexCurve(int nbOdPairs) : 
		IndiffCurveContainer(nbOdPairs) {

};

IndiffCurveContainerConvexCurve::~IndiffCurveContainerConvexCurve() {

};

IndiffCurve* IndiffCurveContainerConvexCurve::createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime) {
	return new ConvexIndiffCurve(minTime, maxTime, maxToll);
};