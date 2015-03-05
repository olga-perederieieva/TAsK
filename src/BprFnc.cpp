#include "BprFnc.h"

#include <cassert>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iomanip>

BprFnc::BprFnc(FPType freeFlow, FPType B, FPType capacity, FPType power) : freeFlow_(freeFlow), 
				 B_(B), capacity_(capacity), power_(power) { 
};

BprFnc::~BprFnc(){

};

FPType BprFnc::evaluate(FPType linkFlow) const {
	if (power_ == 0.0) //in order to avoid problems with floating point arithmatic
		return freeFlow_; 
	return freeFlow_ * (1 + B_ * pow(linkFlow / capacity_, power_));
};

FPType  BprFnc::evaluateDerivative(FPType linkFlow) const {
	if (power_ == 0.0)  //in order to avoid problems with floating point arithmatic
		return 0.0;
	return freeFlow_ * B_ * power_ * pow(linkFlow / capacity_, power_ - 1) / capacity_;
};

void BprFnc::print() const{
	std::cout << "bpr " << std::endl;
	std::cout << "freeFlow = " << freeFlow_ << " B = " << B_ << " capacity = " << capacity_ << 
		" power_ = " << power_ << std::endl;
};

FPType BprFnc::evaluateSecondDerivative(FPType linkFlow) const {
	if (power_ - 1e-16 < 2) return 0.0;  // in order to avoid problems with floating point arithmatic
	return freeFlow_ * B_ * power_ / pow(capacity_, power_) * (power_ - 1) * pow(linkFlow, power_ - 2);
};

FPType BprFnc::evaluateTimeAndDerivative(FPType linkFlow, FPType &der){
	if (power_ == 0.0) {
		der = 0.0;
		return freeFlow_;
	}
	FPType A = freeFlow_ * B_ * pow(linkFlow / capacity_, power_ - 1) / capacity_;
	der = power_ * A;
	return freeFlow_ + A * linkFlow;
};

FPType BprFnc::getCapacity(){
	return capacity_;
};

FncType BprFnc::getType(){
	return BPRFNC;
};