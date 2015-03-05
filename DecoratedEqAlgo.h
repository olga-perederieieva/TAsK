#ifndef _DECOR_EQ_ALGO_
#define _DECOR_EQ_ALGO_

#include "UsedTypes.h"

/** \brief This class is a base class for decorators of EqAlgo. */
class DecoratedEqAlgo {
	public:
		virtual ~DecoratedEqAlgo() {};
		virtual int execute() = 0;

		/** @return total CPU time (in seconds) spent on checking convergence of the algorithm.
		*/
		virtual FPType getTotalConvTime() = 0;

		/** @return total CPU time (in seconds) spent on each iteration of the algorithm (output 
			operations are not included, only pure iteration time).
		*/
		virtual FPType getTotalPureIterTime() = 0;

		virtual FPType getInitialisationTime() = 0;
		
	protected:
		DecoratedEqAlgo(){};
};

#endif
