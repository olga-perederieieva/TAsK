#ifndef DAGRAPH_B_WITH_STEP
#define DAGRAPH_B_WITH_STEP

#include "DAGraphB.h"

class LineSearch;

/** \brief Implements another way of calculating step size for algorithm B
	based on line search.
*/
class DAGraphBWithStep : public DAGraphB {
	public:
		DAGraphBWithStep(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol, 
						int originIndex, bool useMultiStep, LineSearch* lineSearch);
		~DAGraphBWithStep();

	private:

		static LineSearch* lineSearch_;

		virtual FPType calcFlowStep(Path* minPath, Path* maxPath) const;
};

#endif