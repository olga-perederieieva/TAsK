#include "OriginBasedAlgoTapasEqII.h"
#include "OriginBush.h"

OriginBasedAlgoTapasEqII::OriginBasedAlgoTapasEqII(OriginSet *originSet, StarNetwork *net, 
				PASManager* pasSet, AddHook *component, 
				ConvMeasure* conv, FPType timeLimit, int maxNbIter) :
				OriginBasedAlgoTapas(originSet, net, pasSet, component, 
				conv, timeLimit), maxNbIter_(maxNbIter) {

};
	
OriginBasedAlgoTapasEqII::~OriginBasedAlgoTapasEqII(){

};

bool OriginBasedAlgoTapasEqII::mainLoop(OriginBush *bush){
	int nbIter = 0;
	while (true) {
		++nbIter; 
		bool canMoveFlow = OriginBasedAlgo::mainLoop(bush);
		if (!canMoveFlow) return false;
		bush->updateTopSort();
		if (nbIter >= maxNbIter_) return false;
	}; 
	return false;
};