#include "ODSetGPApp3.h"
#include "PathBasedFlowMoveGP.h"
#include "ODMatrix.h"

#include <cassert>

ODMatrix* ODSetGPApp3::mat_ = NULL;
PathBasedFlowMoveGP* ODSetGPApp3::flowMoveGP_ = NULL;

ODSetGPApp3::ODSetGPApp3(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net, 
				ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMoveGP* flowMove, ODMatrix *mat) : 
				ODSet(index, destIndex, originIndex, pathCost, net, shPath, zeroFlow, flowMove) {
	mat_ = mat;
	flowMoveGP_ = flowMove;
}; 

ODSetGPApp3::~ODSetGPApp3() {

}; 

void ODSetGPApp3::calculateProjectedDirection() {
	FPType demand = mat_->getDemandByIndex(originIndex_, destIndex_);
	assert(demand > 0.0);
	flowMoveGP_->setDemand(demand);
}; 

