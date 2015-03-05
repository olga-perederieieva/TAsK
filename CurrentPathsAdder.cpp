#include "CurrentPathsAdder.h"
#include "BiObjLabelContainer.h"
#include "PathSet.h"
#include "Path.h"
#include "ODSet.h"

CurrentPathsAdder::CurrentPathsAdder(const TollContainerType& tolls) : 
			paths_(NULL), tolls_(tolls) {

};

CurrentPathsAdder::~CurrentPathsAdder() {

};

void CurrentPathsAdder::setPathSet(PathSet* paths) {
	paths_ = paths;
};

void CurrentPathsAdder::uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const {
	ODSet* set = paths_->getODSetByIndex(odIndex);
	for (PathIterator pathIt = set->begin(); pathIt != set->end(); ++pathIt) {
		Path* path = *pathIt;
		FPType pathTime = 0;
		TollType pathToll = 0;
		for (ConstStarLinkIterator it = path->begin(); it != path->end(); ++it) {
			StarLink* link = *it;
			pathTime += link->getTime();
			pathToll += tolls_[link->getIndex()];
		}
		BiObjLabel* label = new BiObjLabel(pathTime, pathToll, destIndex, NULL, NULL);
		labels.pushBackLabel(destIndex, label);
	}
};