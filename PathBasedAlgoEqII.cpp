#include "PathBasedAlgoEqII.h"

PathBasedAlgoEqII::PathBasedAlgoEqII(PathSet *pathSet, AddHook *component, 
			ConvMeasure *conv, StarNetwork *net, 
			FPType timeLimit, int maxNbIter, ODMatrix* mat) : 
			PathBasedAlgo(pathSet, component, conv, net, timeLimit, mat), 
			maxNbIter_(maxNbIter) {

}; 

PathBasedAlgoEqII::~PathBasedAlgoEqII() {

}; 

bool PathBasedAlgoEqII::mainLoop(ODSet *odSet) {
	bool isEquilibrated = false;
	int nbIter = 0;
	while (!isEquilibrated) {
		++nbIter;
		isEquilibrated = PathBasedAlgo::mainLoop(odSet);
		if (nbIter >= maxNbIter_) return false;
	}
	return isEquilibrated;
}; 

