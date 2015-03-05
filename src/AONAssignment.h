#ifndef _AON_ASSIGNMENT_
#define _AON_ASSIGNMENT_

#include "UsedTypes.h"

class InitialiseSolution;
class ODMatrix;
class Origin;
class ShortestPath;

/** \brief Abstract class for all-or-nothing assignment.

	This class defines the general structure for all-or-nothing (AON) procedure.
 */
class AONAssignment {
	public:
		
		virtual ~AONAssignment();
		
		/** Defines AON framework and initialises \b solution.
			@param solution an instance of InitialiseSolution class that 
						implements certain initialisation routine that
						depends on solution type.
			@return minimum travel time.
		*/
		FPType execute(InitialiseSolution *solution); 
		
	protected:
		
		ShortestPath* shPath_; /**< pointer to shortest path algorithm */

		/** Constructor.
			@param mat a reference to O-D matrix.
		*/
		AONAssignment(const ODMatrix &mat, ShortestPath* shPath);

		/**	Performs initialisation of \b solution for a given origin node.
			@param solution a pointer to solution.
			@param origin a pointer to an origin node.
			@return minimum travel time calculated for a given origin node.
		*/
		virtual FPType performAssignmentForOrigin(InitialiseSolution *solution, Origin* origin) = 0;

	private:
		const ODMatrix &mat_; /**< reference to O-D matrix.*/
};

#endif
