#include "OriginBasedAlgo.h"
#include "OriginSet.h"
#include "OriginBush.h"

#include <stdlib.h>

OriginBasedAlgo::OriginBasedAlgo(OriginSet *originSet, StarNetwork *net, AddHook *component, 
					ConvMeasure* conv, FPType timeLimit) :
				 	EqAlgo(component, net, timeLimit, conv), originSet_(originSet) { 

};

OriginBasedAlgo::~OriginBasedAlgo(){

};
		
void OriginBasedAlgo::initialise() {
	originSet_->initialise();
};

void OriginBasedAlgo::performOneIteration(){
	for (OriginSetIterator bushIt = originSet_->begin(); bushIt != originSet_->end(); ++bushIt){
		OriginBush* bush = *bushIt;
		bush->updateTopSort();
		bush->improve();
		mainLoop(bush);
	}
	doSmthAfterOrigins(); // hook method - does nothing by default
};

bool OriginBasedAlgo::mainLoop(OriginBush *bush){
	
	bool canMoveFlow = bush->equilibrate(); 
	bush->removeUnusedLinks();
	return canMoveFlow;
};
