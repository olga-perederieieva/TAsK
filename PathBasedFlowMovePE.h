#ifndef PATH_BASED_FLOW_MOVE_PE
#define PATH_BASED_FLOW_MOVE_PE

#include "PathBasedFlowMove.h"

/** \brief This class implements path-based flow move of path equilibration algorithm (approach 3).
	\details For details see \cite Florian1995. 
*/
class PathBasedFlowMovePE : public PathBasedFlowMove {
	public:
		PathBasedFlowMovePE(DescDirectionPath* pathDirectionAlgo);
		~PathBasedFlowMovePE();
		
		/** @return always false.
		*/
		bool executeFlowMove();
};

#endif
