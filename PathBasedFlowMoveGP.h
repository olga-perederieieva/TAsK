#ifndef PATH_BASED_FLOW_MOVE_GP
#define PATH_BASED_FLOW_MOVE_GP

#include "PathBasedFlowMove.h"
#include "UsedTypes.h"

/** \brief This class implements path-based flow move of gradient projection algorithm (approach 3).
	\details For details see \cite Jayakrishnan1994. 
*/
class PathBasedFlowMoveGP : public PathBasedFlowMove {
	public:
		
		/** @param alpha is an algorithm specific constant that influences its convergence.
		*/
		PathBasedFlowMoveGP(FPType alpha, DescDirectionPath* pathDirectionAlgo);
		~PathBasedFlowMoveGP();
		
		/** @return always false.
		*/
		bool executeFlowMove();
		/** Sets demand to the current O-D pair value of demand.
			\note It must be executed each time a new O-D pair is considered.
		*/
		void setDemand(FPType demand);
		
	private:
		const FPType alpha_;
		FPType demand_;
};

#endif
