#ifndef INDIFF_CURVE_CONTAINER_LINEAR_CURVE
#define INDIFF_CURVE_CONTAINER_LINEAR_CURVE 

#include "IndiffCurveContainer.h"

/** \brief Container of linear scalarization functions.
*/
class IndiffCurveContainerLinearCurve : public IndiffCurveContainer {
	public:
		IndiffCurveContainerLinearCurve(int nbOdPairs);
		~IndiffCurveContainerLinearCurve();

	private:
		virtual IndiffCurve* createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime);

};
#endif