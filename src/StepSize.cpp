#include "StepSize.h"
#include "LineSearch.h"
#include "DescDirection.h"

StepSize::StepSize(LineSearch *lineSearch) : lineSearch_(lineSearch) {
	
};

StepSize::~StepSize(){
	
};

FPType StepSize::getStepSize(DescDirection *algo){
	initialiseDerivative(algo);
	return  lineSearch_->execute(0.0, algo->getUpperBound());
};

