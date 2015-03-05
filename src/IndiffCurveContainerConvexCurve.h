#ifndef INDIFF_CURVE_CONTAINER_CONVEX_CURVE
#define INDIFF_CURVE_CONTAINER_CONVEX_CURVE 

#include "IndiffCurveContainer.h"

/** \brief Container of concex scalarization functions.
*/
class IndiffCurveContainerConvexCurve : public IndiffCurveContainer {
	public:
		IndiffCurveContainerConvexCurve(int nbOdPairs);
		~IndiffCurveContainerConvexCurve();

	private:
		virtual IndiffCurve* createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime);

};
#endif