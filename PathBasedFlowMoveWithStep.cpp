#include "PathBasedFlowMoveWithStep.h"
#include "PathAndDirection.h"
#include "Path.h"
#include "StepSize.h"

#include <stdlib.h>
#include <cassert>

PathBasedFlowMoveWithStep::PathBasedFlowMoveWithStep(StepSize *stepSize,
													 DescDirectionPath* pathDirectionAlgo, 
													 FPType zeroFlow):
													 PathBasedFlowMove(pathDirectionAlgo),
													 stepSize_(stepSize), zeroFlow_(zeroFlow),
													 dir_(NULL){

};

PathBasedFlowMoveWithStep::~PathBasedFlowMoveWithStep(){

};

bool PathBasedFlowMoveWithStep::executeFlowMove(){
	assert(dir_ != NULL);
	FPType alpha = stepSize_->getStepSize(dir_);
	if (alpha == 0.0) {
		return true;
	}
	Path* path = NULL;
	FPType newFlow = 0.0;
	for(PathAndDirection *it = beginPathDirection(); it != NULL; it = getNextPathDirection()){
		path = it->getPath();
		newFlow = path->getFlow() + alpha * it->getDirectionValue();
		if (newFlow < zeroFlow_) {
			newFlow = 0.0;

		}
		path->setFlow(newFlow);
	}
	return false;
};

void PathBasedFlowMoveWithStep::setDescDirection(DescDirection* dir){
	dir_ = dir;
};
