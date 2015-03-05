#include "Derivative.h"
#include "LinkFncContainer.h"
#include "LinkFnc.h"

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <cassert>

Derivative::Derivative(FPType zeroFlow, LinkFncContainer *fnc) : 
						size_(0), x_(NULL), y_(NULL),  indexes_(NULL),
						zeroFlow_(zeroFlow), fnc_(fnc) {
	
};

Derivative::~Derivative(){

};

FPType Derivative::calculate(FPType alpha) const {
	
	FPType sum = 0.0;
	FPType inputFlow = 0.0;
	LinkFnc *linkFnc = NULL;
	int index = -1;
	
	for (int i = 0; i < size_; ++i){	
		index = indexes_[i];
		assert(index != -1);
		inputFlow = x_[index] + alpha * y_[index];
		if (inputFlow <= zeroFlow_) inputFlow = 0.0;
		linkFnc = fnc_->getLinkFnc(index);
		sum += y_[index] * linkFnc->evaluate(inputFlow);
	}
	return sum;
};

void Derivative::setDataPointers(int size, FPType *x, FPType *y, int *indexes){
	size_ = size;
	x_ = x;
	y_ = y;
	indexes_ = indexes;
};

int Derivative::getSize() const{
	return size_;
};

FPType Derivative::getX(int index) const{
	assert((index >= 0) && (index < size_));
	return x_[indexes_[index]];
};
		
FPType Derivative::getY(int index) const{
	assert((index >= 0) && (index < size_));
	return y_[indexes_[index]];
};

LinkFnc* Derivative::getLinkFnc(int index) const{
	assert((index >= 0) && (index < size_));
	return  fnc_->getLinkFnc(indexes_[index]);
};

int Derivative::getLinkIndex(int index) const{
	assert((index >= 0) && (index < size_));
	return indexes_[index];
};

FPType Derivative::getZeroFlow() const{
	return zeroFlow_;
};
