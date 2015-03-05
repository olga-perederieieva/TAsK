#ifndef _DESC_DIRECTION_
#define _DESC_DIRECTION_

#include "UsedTypes.h"

class PathBasedFlowMove;

/** \brief This class is a base class that defines the interface for the classes 
	that implement methods used in path-based and link-based algorithms 
	when the step size is calculated throw line search. 
*/
class DescDirection {
	
	public:
		virtual ~DescDirection(){};
		/** @return a pointer to an array with direction of descent specified in
			terms of link flows.
		*/
		virtual FPType* getProjectedDirection() = 0;
		/** @return upper bound for line search. */
		virtual FPType getUpperBound() = 0;
		/** @return pointer to an array with current link flows.*/
		virtual FPType* getLinkFlows() = 0;
		/** @return size of the array with indexes.*/
		virtual int getSize() = 0;
		/** @return pointer to an array with indexes. For example, see Derivative. */
		virtual int* getIndexes() = 0;

		/** This method is used in path-based algorithms.
			@return O-D pair index.*/
		virtual int getOdIndex() const = 0;
		/** This method is used in path-based algorithms.
			@return pointer to object that implements path-based flow move.*/
		virtual PathBasedFlowMove* getPathBasedFlowMove() const = 0;
		
	protected:
		DescDirection(){};
};

#endif
