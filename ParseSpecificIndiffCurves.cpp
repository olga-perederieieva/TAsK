#include "ParseSpecificIndiffCurves.h"
#include "LinearIndiffCurve.h"
#include "ConvexIndiffCurve.h"
#include "ConcaveIndiffCurve.h"
#include "Error.h"

#include <cassert>

ParseSpecificIndiffCurves::ParseSpecificIndiffCurves() {

};

ParseSpecificIndiffCurves::~ParseSpecificIndiffCurves() {

};

IndiffCurve* ParseSpecificIndiffCurves::createCurve(int nbPoints, const PointsContainer& points, 
	const std::string& curveType) {
	assert(nbPoints == 1);
	std::pair<FPType, FPType> values = *points.begin();
	if (curveType == "Linear") {
		return new LinearIndiffCurve(values.first, values.second);
	} else if (curveType == "Convex") {
		return new ConvexIndiffCurve(values.first, values.second);
	} else if (curveType == "Concave") {
		return new ConcaveIndiffCurve(values.first, values.second);
	} else {
		throw Error("Unexpected indifference curve type");
	}
	return NULL;

};