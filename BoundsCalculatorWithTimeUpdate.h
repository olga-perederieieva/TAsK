#ifndef BOUNDS_CALCULATOR_WITH_TIME_UPDATE
#define BOUNDS_CALCULATOR_WITH_TIME_UPDATE 

#include "BoundsCalculatorForBSP.h"

#include <list>

/** \brief Implements travel time bounds based on current flows.
*/
class BoundsCalculatorWithTimeUpdate : public BoundsCalculatorForBSP {
	public:
		BoundsCalculatorWithTimeUpdate(StarNetwork& net, const TollContainerType& tolls);
		~BoundsCalculatorWithTimeUpdate();

		virtual void updateTimeBounds(int destNodeIndex, int odPairIndex);
		virtual FPType getTimeLowerBound(int nodeIndex, int destNodeIndex);

		/** @return link belonging to the shortest path pointing to node destIndex.
		*/
		StarLink* getInComeLink(int destIndex) const;

	private:
		
		int destNode_;
		int prevODIndex_;

};
#endif