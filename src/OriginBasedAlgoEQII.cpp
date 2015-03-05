#include "OriginBasedAlgoEQII.h"
#include "OriginBush.h"

OriginBasedAlgoEQII::OriginBasedAlgoEQII(OriginSet *originSet, StarNetwork *net, AddHook *component, 
		ConvMeasure* conv, FPType timeLimit, int maxNbIter) : 
		OriginBasedAlgo(originSet, net, component, conv, timeLimit), maxNbIter_(maxNbIter) {

}; 

OriginBasedAlgoEQII::~OriginBasedAlgoEQII() {

}; 

bool OriginBasedAlgoEQII::mainLoop(OriginBush *bush) {
	int nbIter = 0;
	while (true) {
		++nbIter; 
		bool canMoveFlow = OriginBasedAlgo::mainLoop(bush);
		if (!canMoveFlow) return false;
		bush->updateTopSort();
		if (!bush->improve()) return false; 
		if (nbIter >= maxNbIter_) return false;
	}; 
	return false;
}; 

