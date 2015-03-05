#ifndef INDIFF_CURVE_CONTAINER_CONCAVE_CURVE
#define INDIFF_CURVE_CONTAINER_CONCAVE_CURVE 

#include "IndiffCurveContainer.h"

/** \brief Container of concave scalarization functions.
*/
class IndiffCurveContainerConcaveCurve : public IndiffCurveContainer {
	public:
		IndiffCurveContainerConcaveCurve(int nbOdPairs);
		~IndiffCurveContainerConcaveCurve();

	private:
		virtual IndiffCurve* createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime);

};
#endif