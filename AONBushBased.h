#ifndef AON_BUSH_BASED
#define AON_BUSH_BASED value

#include "AONAssignment.h"
class ShortestPath;

/** \brief All-or-nothing assignment for bush-based algorithms.

	In the case of bush-based methods, AON has to iterate through
	all nodes, not only destination nodes to calculate minimum and maximum
	costs.
*/
class AONBushBased : public AONAssignment {
	public:
		AONBushBased(const ODMatrix &mat, ShortestPath* shPath, int nbNodes);
		~AONBushBased();

	protected:
		/** @return always zero, for bush-based methods we do not calculate total minimum time.*/
		virtual FPType performAssignmentForOrigin(InitialiseSolution *solution, 
			Origin* origin);

	private:
		int nbNodes_;
};

#endif