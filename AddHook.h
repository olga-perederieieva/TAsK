#ifndef ADD_HOOK_
#define ADD_HOOK_

#include "UsedTypes.h"

/** \brief A base class that is used in EqAlgo class to perform some operations with 
	convergence information available after each iteration of the algorithm. 
	\details Usually such operations include screen or file output. This class does nothing.
	Specific operations are implemented in derived classes.
 */
class AddHook {
	public:
		
		AddHook(){};
		virtual ~AddHook(){};
		
		/** This method does nothing. */
		virtual void produceOut(FPType timePassed, FPType gap) {};
		
};

#endif
