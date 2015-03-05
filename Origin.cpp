#include "Origin.h"
#include "PairOD.h"

#include <cassert>
#include <stdlib.h>

Origin::Origin(int index) : index_(index) {
};

Origin::~Origin(){
	for (PairODIterator jt = begin(); jt != end(); ++jt) {
		delete *jt;
	}
};

void Origin::addDestination(PairOD *dest){
	assert(dest != NULL);
	destList_.push_front(dest);
};
	
PairODIterator Origin::begin(){
	return destList_.begin();
};	

PairODIterator Origin::end(){
	return destList_.end();
};

int Origin::getIndex() const{
	return index_;
};

bool Origin::isEmpty() const{
	return destList_.empty();
};

int Origin::getNbDest() const{
	return destList_.size();
};
