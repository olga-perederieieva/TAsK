#ifndef AON_USUAL
#define AON_USUAL

#include "AONAssignment.h"

class ShortestPath;
class PairOD;

/** \brief Implements basic all-or-nothing procedure for a given origin node.
*/
class AONUsual : public AONAssignment {
	public:
		AONUsual(const ODMatrix &mat, ShortestPath* shPath);
		~AONUsual();

	protected:
		/** Calculates travel time for a given O-D pair: demand multiplied by 
			the shortest path distance between given O-D pair.
			@param dest O-D pair object.
			@return total travel time for a given O-D pair.
		*/
		virtual FPType calculateTravelTime(const PairOD& dest) const;

	private:

		virtual FPType performAssignmentForOrigin(InitialiseSolution *solution, Origin* origin);
};

#endif