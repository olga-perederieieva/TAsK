#include "OriginBasedAlgoTapas.h"


OriginBasedAlgoTapas::OriginBasedAlgoTapas(OriginSet *originSet, StarNetwork *net, PASManager* pasSet, 
						AddHook *component, ConvMeasure* conv, FPType timeLimit) : 
						OriginBasedAlgo(originSet, net, component, conv, timeLimit), 
						pasSet_(pasSet) {

}; 

OriginBasedAlgoTapas::~OriginBasedAlgoTapas() {

}; 

void OriginBasedAlgoTapas::doSmthAfterOrigins() {
	pasSet_->deleteUnusedPASAndMoveFlow();
}; 

