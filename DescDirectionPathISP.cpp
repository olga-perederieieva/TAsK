#include "DescDirectionPathISP.h"
#include "StarLink.h"

#include <cassert>

DescDirectionPathISP::DescDirectionPathISP(FPType slope, FPType scaleFact, FPType delta):
                        DescDirectionPath(delta), 
												slope_(slope), scaleFact_(scaleFact) {
};

DescDirectionPathISP::~DescDirectionPathISP(){

};
		
PathAndDirection** DescDirectionPathISP::createPathDirection(int &size, const std::list<Path*> &paths,
                                              bool &isEquilibrated){
	
  	// find shortest and longest paths
  	Path *minPath = findMinPath(paths);
  	FPType minDist = minPath->getCurrCost();
  	Path *maxPath = findMaxPath(paths, minPath);
  	FPType maxDist = maxPath->getCurrCost();
  	
  	// check is O-D is equilibrated
  	if (maxDist - minDist <= delta_){
  		isEquilibrated = true;
  		size = 0;
  		return NULL;
  	}
  	
    isEquilibrated = false;
    size = paths.size();
    // allocate memory
    PathAndDirection **returnSet = new PathAndDirection*[size];
    for (int i = 0; i < size; ++i) {
      returnSet[i] = new PathAndDirection();
    }

  	// calculate direction of descent
  	FPType thr = calculateThreshold(minDist, maxDist);
  	FPType pathCost = 0.0;
  	int count = 0;
  	FPType dir = 0.0;
  	FPType totalDir = 0.0;
  	Path *curPath = NULL;
  	int countCheap = 0;
  	int indexes[size];
  	for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
  		curPath = *it;
  		assert(curPath != NULL);
  		pathCost = curPath->getCurrCost();
  		if (pathCost > thr) { //set of costlier paths
  			dir = minDist - pathCost;
  			totalDir -= dir;
  		} else {
  			dir = 0.0; 
  			indexes[countCheap] = count;
  			++countCheap;
  		}
  		(returnSet[count])->setDirection(dir);
  		(returnSet[count])->setPath(curPath);
  		++count;
  	}
  	
  	// calculate slopes and total slope first
  	FPType totalSlope = 0.0;
  	FPType curSlope = 0.0;
  	FPType slopes[countCheap];
  	for (int i = 0; i < countCheap; ++i){
  		curSlope = calculateDerivative((returnSet[indexes[i]])->getPath());
  		if (curSlope < slope_) curSlope = slope_;
  		slopes[i] = curSlope;
  		totalSlope += 1.0 / curSlope;
  	}
  	// calculate direction of cheaper paths
  	for (int i = 0; i < countCheap; ++i){
  		count = indexes[i];
  		dir = totalDir / (slopes[i] * totalSlope);
  		(returnSet[count])->setDirection(dir);
  	}
	return returnSet;
};

FPType DescDirectionPathISP::calculateThreshold(FPType minDist, FPType maxDist) const{
	return minDist + scaleFact_ * (maxDist - minDist);
};

FPType DescDirectionPathISP::calculateDerivative(Path* path) const{
	FPType der = 0.0;
	FPType flow = 0.0;
	for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
    StarLink* link = *it;
		flow = link->getFlow();
		if (flow < slope_) {
			flow = slope_;
		} 
		der +=  link->getDerivative();
	}
	return der;
};
