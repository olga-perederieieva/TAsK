#ifndef STEP_SIZE
#define STEP_SIZE

#include "UsedTypes.h"

class LineSearch;
class DescDirection;

/** \brief This class is used in path-based algorithms that use a line search in order to
	calculate a step size. This class provides interface for step size calculation.
*/
class StepSize {
	public:
		
		virtual ~StepSize();
		
		/** This method returns a calculated step size given a DescDirection object 
			that provides access to the data necessary for a line search.
		*/
		FPType getStepSize(DescDirection *algo);
		
	protected:
		
		LineSearch *lineSearch_;

		StepSize(LineSearch *lineSearch);

		/** This method initialises derivative used in line search.
		*/
		virtual void initialiseDerivative(DescDirection *algo) = 0;
};

#endif
