#include "Bisection.h"
#include "Derivative.h"

#include <cassert>
#include <math.h>
#include <stdlib.h>

Bisection::Bisection(FPType precision, Derivative *der) : 
						LineSearch(der), precision_(precision){
	assert(precision > 0.0 && der != NULL);
};

Bisection::~Bisection(){

};

FPType Bisection::execute(FPType a, FPType b){
	
	assert(a <= b);

	FPType alpha = 0.0;
	FPType der = der_->calculate(b);
	if (der <= 0.0) {
		return b; 
	}
	if (der_->calculate(0.0) >= 0.0) {
		return 0.0;
	}
	
	while (true) {
		alpha = (a + b) / 2.0;
		if (alpha == a || alpha == b) {
			
			return alpha; // to prevent looping
		}
		der = der_->calculate(alpha);
		if (der <= 0.0) {
			a = alpha;
		} else {
			b = alpha;
		};
		if (((b - a) <= 2 * precision_)){ 
			break;
		}
	}
	alpha = (a + b) / 2.0;
	return alpha;
};

FPType Bisection::getPrecision() const {
	return precision_;
};
