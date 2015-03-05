#ifndef PARSE_SPECIFIC_INDIFF_CURVES
#define PARSE_SPECIFIC_INDIFF_CURVES 

#include "ParseIndiffCurves.h"

/** \brief Parser for linear, convex and concave scalarization functions.
*/
class ParseSpecificIndiffCurves : public ParseIndiffCurves {
	public:
		ParseSpecificIndiffCurves();
		~ParseSpecificIndiffCurves();

	protected:
		/** Allocates memory for a scalarization function specified by curveType.
			Error is thrown if curveType is not linear, convex or concave.
		*/
		virtual IndiffCurve* createCurve(int nbPoints, const PointsContainer& points,
			const std::string& curveType);

};
#endif