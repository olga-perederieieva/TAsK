#include "PASWithStep.h"
#include "Derivative.h"
#include "StarLink.h"
#include "LineSearch.h"

#include <stdlib.h>

PASWithStep::PASWithStep(FPType zeroFlow, FPType dirTol, LineSearch* lineSearch, 
				int nbLinks) : 
				PAS(zeroFlow, dirTol), lineSearch_(lineSearch), nbLinks_(nbLinks) {

};

PASWithStep::~PASWithStep(){

};

FPType PASWithStep::getFlowShift(){
	FPType x[nbLinks_];
	FPType y[nbLinks_];
	int indexes[nbLinks_];

	int size = 0;
	FPType dir[2]; 
	dir[cheapSegm_] = expCost_ - cheapCost_;
	dir[1 - cheapSegm_] = -dir[cheapSegm_];
	StarLink* link = NULL;
	int linkIndex = -1;
	for (int i = 0; i < 2; ++i) {
		for (std::list<StarLink*>::const_iterator it = segments_[i].begin();
			 it != segments_[i].end(); ++it) {
			link = *it;
			linkIndex = link->getIndex();
			x[linkIndex] = link->getFlow();
			y[linkIndex] = dir[i];
			indexes[size] = linkIndex;
			++size;
		}
	}
	(lineSearch_->getDerivative())->setDataPointers(size, x, y, indexes);
	return lineSearch_->execute(0.0, totalShift_ / dir[cheapSegm_]) * dir[cheapSegm_];

};