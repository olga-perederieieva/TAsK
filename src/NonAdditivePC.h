#ifndef _NON_ADDITIVE_PATH_COST_
#define _NON_ADDITIVE_PATH_COST_

#include "PathCost.h"
#include "IndiffCurveContainer.h"

/** \brief This class implements non-additive path cost calculation.
*/
class NonAdditivePC : public PathCost {
	public:
		NonAdditivePC(const IndiffCurveContainer& curves, const TollContainerType& tolls);
		~NonAdditivePC();
		/** @return non-additive path cost given path and O-D pair index.
		*/
		FPType calculate(Path *path, int odIndex) const;
		/** @return non-additive path cost given total travel time, toll and O-D pair index.
		*/
		FPType calculate(FPType totalTime, TollType totalToll, int odIndex) const;
		/** @return value of scalarization function given toll and O-D pair index.
		*/
		FPType evaluateCurve(TollType totalToll, int odIndex) const;

	private:
		const IndiffCurveContainer& curves_;
		const TollContainerType& tolls_;
};

#endif
