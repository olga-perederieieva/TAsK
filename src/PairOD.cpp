#include "PairOD.h"

#include <iostream>

PairOD::PairOD(int index, FPType demand) : index_(index), demand_(demand),  odIndex_(-1) {

};

PairOD::~PairOD(){

};

void PairOD::print(){
	std::cout << "odIndex = " << odIndex_ << " destIndex = " << index_ << " demand = " 
		<< demand_  << std::endl; 
};

FPType PairOD::getDemand() const{
	return demand_;
};

int PairOD::getIndex() const{
	return index_;
};
	
int PairOD::getODIndex() const{
	return odIndex_;
};

void PairOD::setODIndex(int index){
	odIndex_ = index;
};
