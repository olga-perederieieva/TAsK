#ifndef _ARMIJO_
#define _ARMIJO_

#include "LineSearch.h"

class Derivative;

/** \brief This class implements Armijo-like rule to calculate the step size.
*/
class Armijo : public LineSearch {
	
	public:
		/** 
			@param decrement the factor by which the step size will be decreased at each iteration.
			@param der object that implements derivative calculation.
		*/	
		Armijo(FPType decrement, Derivative* der);
		~Armijo();
		
		FPType execute(FPType a, FPType b);
	
		/** @return the factor by which the step size is decreased.
		*/
		FPType getDecriment() const;
	
	private:
	
		const FPType decriment_;
	
};

#endif

