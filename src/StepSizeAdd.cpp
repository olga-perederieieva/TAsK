#include "StepSizeAdd.h"
#include "DescDirection.h"
#include "Derivative.h"

StepSizeAdd::StepSizeAdd(LineSearch *lineSearch, Derivative *der) : 
			StepSize(lineSearch), der_(der){

};

StepSizeAdd::~StepSizeAdd(){

};

void StepSizeAdd::initialiseDerivative(DescDirection *algo){
	der_->setDataPointers(algo->getSize(), algo->getLinkFlows(),
						  algo->getProjectedDirection(), algo->getIndexes());
};
