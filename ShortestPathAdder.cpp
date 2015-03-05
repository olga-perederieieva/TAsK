#include "ShortestPathAdder.h"
#include "PathSet.h"
#include "BiObjLabelContainer.h"
#include "Path.h"
#include "ODSet.h"

#include <limits>

ShortestPathAdder::ShortestPathAdder(const TollContainerType& tolls) : CurrentPathsAdder(tolls) {

};

ShortestPathAdder::~ShortestPathAdder() {

};

void ShortestPathAdder::uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const {
	ODSet* set = paths_->getODSetByIndex(odIndex);
	FPType minCost = std::numeric_limits<FPType>::infinity();
	Path* minPath = NULL;
	for (PathIterator pathIt = set->begin(); pathIt != set->end(); ++pathIt) {
		Path* path = *pathIt;
		FPType pathCost = path->getCurrCost();
		if (pathCost < minCost) {
			minCost = pathCost;
			minPath = path;
		}
	}
	if (minPath != NULL) {
		FPType pathTime = 0;
		TollType pathToll = 0;
		for (StarLinkIterator it = minPath->begin(); it != minPath->end(); ++it) {
			StarLink* link = *it;
			pathTime += link->getTime();
			pathToll += tolls_[link->getIndex()];
		}
		BiObjLabel* label = new BiObjLabel(pathTime, pathToll, destIndex, NULL, NULL);
		labels.pushBackLabel(destIndex, label);
	}
	
};