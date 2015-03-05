#ifndef NEW_BISECTION_
#define NEW_BISECTION_

#include "LineSearch.h"

class Derivative;

/** \brief This class implements bisection for step size calculation. 
*/
class Bisection : public LineSearch {
	
	public:
		
		/** 
			@param precision precision of bisection.
			@param der pointer to the object that implements derivative calculation.
		*/
		Bisection(FPType precision, Derivative *der);
		~Bisection();
	
		FPType execute(FPType a, FPType b);

		/** @return precision of bisection.
		*/
		FPType getPrecision() const;
	
	private:
		const FPType precision_;
};

#endif

