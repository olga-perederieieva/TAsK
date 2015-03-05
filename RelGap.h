#ifndef REL_GAP
#define REL_GAP

#include "ConvMeasure.h"

class StarNetwork;
class ODMatrix;
class ShortestPath;

/** \brief This class implements relative gap. 
	\details \f$ RGAP = rGap = 1 - \frac{\sum_{p \in Z}D_p \cdot C_{min}^p}{\sum_{a \in A} f_a \cdot c_a}\f$
	where  \f$C_{min}^p = \min_{k \in K_p} C_k \f$ is the shortest path of O-D pair \f$ p \f$.
*/
class RelGap : public ConvMeasure{
	
	public:

		/** @param precision relative gap precision for deciding when to stop calculations.
			@param net network.
			@param mat O-D matrix.
			@param shPath shortest path algorithm.
		*/
		RelGap(FPType precision, StarNetwork *net, ODMatrix *mat, ShortestPath *shPath);
		~RelGap();
		
	private:
		
		StarNetwork *net_;
		ODMatrix *mat_;
		ShortestPath *shPath_;
		
		FPType getMinTravelTime();
		FPType getTotalTravelTime();
		
		/** If total travel time is close to zero (\f$ \leq 10^{-25}\f$), this method returns 
			infinity. It is assumed that in this case the problem is not feasible. It occurs only in 
			Frank-Wolf methods (see FWAlgo) on first iteration because link flows are not yet updated
			after AON assignment but minimum travel time is already calculated.
		*/
		FPType calculateGapValue(FPType minTravelTime, FPType totalTravelTime);

		FPType calculateGap();
		FPType calculateGap(FPType minTravelTime);
		
};

#endif
