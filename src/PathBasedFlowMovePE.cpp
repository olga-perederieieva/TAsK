#include "PathBasedFlowMovePE.h"
#include "PathAndDirection.h"
#include "Path.h"

PathBasedFlowMovePE::PathBasedFlowMovePE(DescDirectionPath* pathDirectionAlgo):
										 PathBasedFlowMove(pathDirectionAlgo){

};

PathBasedFlowMovePE::~PathBasedFlowMovePE(){

};
		
bool PathBasedFlowMovePE::executeFlowMove(){
	Path *maxPath = (pathDirection_[0])->getPath();
	Path *minPath = (pathDirection_[1])->getPath();
	FPType maxDir = (pathDirection_[0])->getDirectionValue();
	FPType flow = maxPath->getFlow();
	if (flow < -maxDir) {
		maxPath->setFlow(0.0);
		minPath->setFlow(minPath->getFlow() + flow);
	} else {
		maxPath->setFlow(flow + maxDir);
		minPath->setFlow(minPath->getFlow() - maxDir);
	}
	return false;
};
