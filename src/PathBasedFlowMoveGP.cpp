#include "PathBasedFlowMoveGP.h"
#include "PathAndDirection.h"
#include "Path.h"

#include <stdlib.h>

PathBasedFlowMoveGP::PathBasedFlowMoveGP(FPType alpha, DescDirectionPath* pathDirectionAlgo) : 
			PathBasedFlowMove(pathDirectionAlgo), alpha_(alpha), demand_(0.0) {

};

PathBasedFlowMoveGP::~PathBasedFlowMoveGP(){

};

void PathBasedFlowMoveGP::setDemand(FPType demand){
	demand_ = demand;
};

bool PathBasedFlowMoveGP::executeFlowMove(){
	
	FPType totalFlowToMove = 0.0;
	Path *currPath = NULL;
	FPType currDir = 0.0;
	FPType flow = 0.0;
	for (int i = 0; i < size_ - 1; ++i) {
		currPath = (pathDirection_[i])->getPath();
		currDir = (pathDirection_[i])->getDirectionValue();
		if (currPath->getFlow() < -alpha_ * currDir) {
			currPath->setFlow(0.0);
		} else {
			flow = currPath->getFlow() + alpha_ * currDir;
			currPath->setFlow(flow);
			totalFlowToMove += flow;
		}
	}
	Path *minPath = (pathDirection_[size_ - 1])->getPath();
	minPath->setFlow(demand_ - totalFlowToMove);
	return false;
};
