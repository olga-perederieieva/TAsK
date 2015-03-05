#include "FWAlgo.h"
#include "Timer.h"
#include "ConvMeasure.h"
#include "LinkFlows.h"

FWAlgo::FWAlgo(LinkFlows *linkFlows, AddHook *component, 
			StarNetwork *net, ConvMeasure* conv, FPType timeLimit) :
			EqAlgo(component, net, timeLimit, conv), 
			linkFlows_(linkFlows) {

};

FWAlgo::~FWAlgo(){

};

void FWAlgo::initialise(){
	linkFlows_->initialise();
};

void FWAlgo::performOneIteration(){
	// Update
	linkFlows_->updateLinkFlows();
	// Direction finding 
	linkFlows_->equilibrate(); //line search and flow shift
};

bool FWAlgo::isConverged(){
	return conv_->isConverged(linkFlows_->getMinTravelTime());
};