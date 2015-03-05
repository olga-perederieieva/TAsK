#include "PathBasedAlgo.h"
#include "ODSet.h"
#include "PathSet.h"

PathBasedAlgo::PathBasedAlgo(PathSet *pathSet, AddHook *component, ConvMeasure *conv,
						StarNetwork *net, FPType timeLimit, ODMatrix* mat) :  
						EqAlgo(component, net, timeLimit, conv), 
						pathSet_(pathSet), printPathSet_(mat) {

	
};

PathBasedAlgo::~PathBasedAlgo(){

};

bool PathBasedAlgo::mainLoop(ODSet *odSet){
	odSet->updateSet();
	bool wasImproved = odSet->improveSet();
	if (wasImproved || (odSet->getNbPaths() > 1)) {
		if (odSet->equilibrate()) {
			odSet->removeUnusedPaths();
			return true;
		}
		odSet->projectPathFlowOnLinks();
		odSet->removeUnusedPaths();
	} else { 
		return true;
	}
	return false;
};

void PathBasedAlgo::initialise(){
	pathSet_->initialise();
	
};

void PathBasedAlgo::performOneIteration(){
	for (ConstODSetIterator odSetIt = pathSet_->begin(); odSetIt != pathSet_->end(); ++odSetIt){
		mainLoop(*odSetIt);
	}
};