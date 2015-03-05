#include "ODMatrix.h"
#include "Origin.h"
#include "PairOD.h"

#include <cassert>
#include <iostream>
#include <sstream>

ODMatrix::ODMatrix(int nbNodes, int nbOrigins) : nbNodes_(nbNodes), 
					nbODPairs_(0), odMatrix_(nbOrigins), demandByIndex_() {
	assert(nbNodes > 0);
};

ODMatrix::~ODMatrix(){
	for (std::vector<Origin*>::iterator it = odMatrix_.begin(); it != odMatrix_.end(); ++it){
		delete *it;
	}
};

int ODMatrix::getNbOrigins() const{
	return odMatrix_.size(); 
};

void ODMatrix::addOrigin(Origin *origin){
	assert(getNbOrigins() <= nbNodes_ && origin->getIndex() >= 0);
	odMatrix_[origin->getIndex()] = origin;
};

OriginIterator ODMatrix::begin(){
	return odMatrix_.begin();
};


OriginIterator ODMatrix::end(){
	return odMatrix_.end();
};

ConstOriginIterator ODMatrix::begin() const {
	return odMatrix_.begin();
};

ConstOriginIterator ODMatrix::end() const {
	return odMatrix_.end();
};

void ODMatrix::print(){
	std::cout << "nb pairs = " << nbODPairs_ << std::endl;
	for (OriginIterator it = begin(); it != end(); ++it){
		Origin* origin = *it; 
		std::cout << "Origin: " << origin->getIndex() << std::endl;
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
			PairOD* dest = *jt;
			dest->print();
		}
	};
};

void ODMatrix::setIndexes(){
	int count = 0;
	int originIndex = -1;
	//
	for (OriginIterator it = begin(); it != end(); ++it){
		Origin* origin = *it; 
		originIndex = origin->getIndex(); 
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
			PairOD* dest = *jt;
			dest->setODIndex(count);
			demandByIndex_.insert(std::make_pair<std::string, FPType>(
					createKey(originIndex, dest->getIndex()), 
							  dest->getDemand()));
			++count;
		}
	}
	nbODPairs_ = count;
	std::cout << "nbODPairs = " << nbODPairs_ << std::endl;	
};

FPType ODMatrix::getDemandByIndex(int originIndex, int destIndex) const{
	std::tr1::unordered_map<std::string, FPType>::const_iterator got = demandByIndex_.find(createKey(originIndex, destIndex));
	if (got == demandByIndex_.end()) return 0.0;
	return got->second; 
};

int ODMatrix::getNbODPairs() const{
	return nbODPairs_;
};

std::string ODMatrix::createKey(int originIndex, int destIndex) const{
	std::stringstream key;
	key << originIndex << "_" << destIndex;
	return key.str();
};

Origin* ODMatrix::getOriginByIndex(int index) const {
	assert(index >= 0 && index < getNbOrigins());
	return odMatrix_[index];
};
