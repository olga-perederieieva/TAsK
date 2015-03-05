#ifndef STEP_SIZE_ADDITIVE 
#define STEP_SIZE_ADDITIVE

#include "StepSize.h"

class Derivative;

/** \brief This class implements step size calculation in the case when 
	additive path cost function is used.
*/
class StepSizeAdd : public StepSize {
	public:
		StepSizeAdd(LineSearch *lineSearch, Derivative *der);
		~StepSizeAdd();
		
		void initialiseDerivative(DescDirection *algo);
	private:
		Derivative *der_;
};

#endif
