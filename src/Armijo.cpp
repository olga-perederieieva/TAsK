#include "Armijo.h"
#include "Derivative.h"

#include <cassert>
#include <stdlib.h>

Armijo::Armijo(FPType decriment, Derivative *der) : 
			LineSearch(der), decriment_(decriment) {
	assert(decriment > 0.0 && der != NULL);
};

Armijo::~Armijo(){

};

FPType Armijo::execute(FPType a, FPType b){
	assert(a <= b);
	FPType alpha = b;
	if (der_->calculate(0.0) >= 0.0) {
		return 0.0; // patological case when derivative is positive in 0.0
	}
	FPType der = 0.0;
	while (true) {
		der = der_->calculate(alpha);
		if (der > 0.0) {
			alpha /= decriment_; 
		} else {
			
			return alpha;
		}
		
		
	}
	return alpha;
};

FPType Armijo::getDecriment() const{
	return decriment_;
};
