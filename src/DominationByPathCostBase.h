#ifndef DOMINATION_BY_PATH_COST_BASE
#define DOMINATION_BY_PATH_COST_BASE 

#include "UsedTypes.h"

class BiObjLabelContainer;

class BiObjLabel;

/** \brief Base class for path cost domination rule.

	Defines interface for derived classes and provides default implementation that doesn't do anything.
*/
class DominationByPathCostBase {
	public:
		DominationByPathCostBase();
		virtual ~DominationByPathCostBase();

		/** Adds label to labels.
		*/
		virtual void addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
			BiObjLabel* label, int destIndex, int odIndex);

		/** Doesn't do anything.
		*/
		virtual void resetBestKnownPathCost();

		/** Performs usual dominance check.
			@return true if label based on timeLowerBound and tollLowerBound is not
				dominated by any label in the destination node, false otherwise.
		*/
		virtual bool isWorth(const BiObjLabelContainer& labels, int destIndex, 
			FPType timeLowerBound, TollType tollLowerBound, int odIndex) const;

		/** Doesn't do anything.
		*/
		virtual void updateBestKnown(const BiObjLabelContainer& labels, int destIndex, 
			int odIndex);

};
#endif