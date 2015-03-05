#ifndef LINE_SEARCH
#define LINE_SEARCH

#include "UsedTypes.h"

class Derivative;

/** \brief This class is a base class for different line search strategies.
*/
class LineSearch {
	
	public:
	
		virtual ~LineSearch(){};

		/** This method performs a line search. 
			@param a lower bound for line search;
			@param b upper bound.
		*/
		virtual FPType execute(FPType a, FPType b) = 0;

		/** @return a pointer to the derivative object. It is sometimes needed by the algorithms
			in order to set data pointers of Derivative.
		*/
		Derivative* getDerivative() const {return der_;};
		
	protected:
		
		Derivative *der_;

		LineSearch(Derivative *der) : der_(der) {};

};

#endif
