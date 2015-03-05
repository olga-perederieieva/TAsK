#ifndef BOUNDS_CALCULATOR_FOR_BSP
#define BOUNDS_CALCULATOR_FOR_BSP 

#include "LabelCorrectingAlWithoutZonesCheck.h"
#include "BoundsCalculatorForBSPBase.h"

/** \brief Implements travel time bound calculation based on zero flows.

	Toll lower bounds are also calculated in this class.
*/
class BoundsCalculatorForBSP : public BoundsCalculatorForBSPBase {
	public:
		BoundsCalculatorForBSP(StarNetwork& net, const TollContainerType& tolls);

		/** Calculates all bounds (travel time based on zero flows and toll):
			from every node to every destination node.
		*/
		virtual void initializeBounds();

		~BoundsCalculatorForBSP();

		/** Hook method for children classes to update travel time bounds
			when necessary.
		*/
		virtual void updateTimeBounds(int destNodeIndex, int odPairIndex);
		/** @return toll lower bound from node nodeIndex to destination destNodeIndex.
		*/
		virtual TollType getTollLowerBound(int nodeIndex, int destNodeIndex);
		/** @return zero flow travel time lower bound from node nodeIndex to destination destNodeIndex.
		*/
		virtual FPType getTimeLowerBound(int nodeIndex, int destNodeIndex);

	protected:
		LabelCorrectingAlWithoutZonesCheck* spp_;
		std::vector<FPType> zeroFlowTimes_;
		std::vector<TollType> tolls_;
		StarNetwork* reverseStar_;
		StarNetwork* originalNet_;
		const TollContainerType& linkTolls_;

		/** Creates reversed network for bounds calculation.
		*/
		virtual StarNetwork* getNetwork(StarNetwork& net);
		/** Creates reversed network with tolls as link costs for bounds calculation.
		*/
		virtual StarNetwork* getNetworkWithTolls(StarNetwork& net, const TollContainerType& tolls);
		/** @return pointer to the network.
		*/
		virtual StarNetwork* getOppositeNet();

};

#endif