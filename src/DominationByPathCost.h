#ifndef DOMINATION_BY_PATH_COST
#define DOMINATION_BY_PATH_COST 

#include "DominationByPathCostBase.h"

class NonAdditivePC;

/** \brief This class implements path cost domination rule.
*/
class DominationByPathCost : public DominationByPathCostBase {

	public:
		/** @param pathCost object that implements non-additive path cost
				calculation routine.
		*/
		DominationByPathCost(const NonAdditivePC& pathCost);
		~DominationByPathCost();

		/** Adds label label to labels and updates best known path cost
			if nodeIndex is equal to destIndex and cost of path is lower than
			the best known path cost.
		*/
		virtual void addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
			BiObjLabel* label, int destIndex, int odIndex);
		/** Initialises best known path cost with infinity.
		*/
		virtual void resetBestKnownPathCost();
		/** @return true if path cost based on timeLowerBound and tollLowerBound
			is less than best known path cost, false otherwise.
		*/
		virtual bool isWorth(const BiObjLabelContainer& labels, int destIndex, 
			FPType timeLowerBound, TollType tollLowerBound, int odIndex) const;
		/** Updates best known path cost: iterates through all labels stored in the 
			destination node, checks if any path cost of these labels is lower
			than the best known path cost and updates best known path cost if it is true.
		*/
		virtual void updateBestKnown(const BiObjLabelContainer& labels, int destIndex, 
			int odIndex);

	private:
		const NonAdditivePC& pathCost_;
		FPType bestKnownPathCost_;
};
#endif