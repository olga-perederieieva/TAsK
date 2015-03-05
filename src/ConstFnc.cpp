#include "ConstFnc.h"

#include <stdlib.h>
#include <iostream>

ConstFnc::ConstFnc(FPType param) : param_(param){

};

ConstFnc::~ConstFnc(){

};
		
FPType ConstFnc::evaluate(FPType linkFlow) const {
	return param_;
};

FPType ConstFnc::evaluateDerivative(FPType linkFlow) const {
	return 0.0;
};

void ConstFnc::print() const{
	std::cout << "ConstFnc" << std::endl;
	std::cout << "param = " << param_ << std::endl;
};

FncType ConstFnc::getType(){
	return CONSTFNC;
};

FPType ConstFnc::evaluateTimeAndDerivative(FPType linkFlow, FPType &der){
	der = 0.0;
	return param_;
};
