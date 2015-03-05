#ifndef BOUNDS_CALCULATOR_FOR_BSP_BASE
#define BOUNDS_CALCULATOR_FOR_BSP_BASE 

#include "UsedTypes.h"

/** \brief Defines interface for other bounds calculator, implements the case when no bounds
	are calculated.
*/
class BoundsCalculatorForBSPBase {
	public:
		BoundsCalculatorForBSPBase();
		virtual ~BoundsCalculatorForBSPBase();

		/** Doesn't do anything.
		*/
		virtual void initializeBounds();

		/** Doesn't do anything.
		*/
		virtual void updateTimeBounds(int destNodeIndex, int odPairIndex);

		/** @return always zero.
		*/
		virtual TollType getTollLowerBound(int nodeIndex, int destNodeIndex);
		/** @return always zero.
		*/
		virtual FPType getTimeLowerBound(int nodeIndex, int destNodeIndex);

};
#endif