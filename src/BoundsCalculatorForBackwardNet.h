#ifndef BOUNDS_CALCULATOR_FOR_BACKWARD_NET
#define BOUNDS_CALCULATOR_FOR_BACKWARD_NET 

#include "BoundsCalculatorForBSP.h"

/** \brief Implements travel time and toll bound calculation for reversed network.
*/
class BoundsCalculatorForBackwardNet : public BoundsCalculatorForBSP {
	public:
		BoundsCalculatorForBackwardNet(StarNetwork& net, const TollContainerType& tolls);
		~BoundsCalculatorForBackwardNet();

	protected:

		/** @return creates and returns reverse network.
		*/
		virtual StarNetwork* getNetwork(StarNetwork& net);
		/** Creates reversed network based on tolls as link costs.
		*/
		virtual StarNetwork* getNetworkWithTolls(StarNetwork& net, const TollContainerType& tolls);
		/** @return reversed network.
		*/
		virtual StarNetwork* getOppositeNet();

	private:
		StarNetwork* net_;
		StarNetwork* backwardNet_;
};
#endif