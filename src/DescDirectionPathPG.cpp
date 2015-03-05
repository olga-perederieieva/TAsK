#include "DescDirectionPathPG.h"

#include <math.h>
#include <limits>
#include <cassert>

DescDirectionPathPG::DescDirectionPathPG(FPType delta) : DescDirectionPath(delta) {

};

DescDirectionPathPG::~DescDirectionPathPG(){

};

PathAndDirection** DescDirectionPathPG::createPathDirection(int &size, const std::list<Path*> &paths, 
      bool &isEquilibrated){	
  	  
  // find average dist first
  FPType avDist = 0.0;
  FPType minDist = std::numeric_limits<FPType>::infinity( );
  FPType cost = 0.0;
  FPType maxDist = 0.0;
 
  for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
  	cost = (*it)->getCurrCost();
  	avDist += cost;
  	if (cost < minDist) {
      minDist = cost;
    }
  	if (cost > maxDist) {
      maxDist = cost;
    }
  }
  
  if (maxDist - minDist <= delta_) {
  	isEquilibrated = true;
  	size = 0;
  	return NULL;
  }

  isEquilibrated = false;
  size = paths.size();
  PathAndDirection **returnSet = new PathAndDirection*[size];
  for (int i = 0; i < size; ++i) {
    returnSet[i] = new PathAndDirection();
  }

  avDist /= static_cast<FPType>(size);
  int i = 0;
  FPType dirVal = 0.0;
  Path* path = NULL;
  FPType dirSum = 0;
  for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
      path = *it;
    	(returnSet[i])->setPath(path);
    	dirVal = avDist - path->getCurrCost();
    	(returnSet[i])->setDirection(dirVal);
      if (i != size-1) dirSum += dirVal;
    	++i;
  }
  (returnSet[size-1])->setDirection(-dirSum); // this stuff fixes converegence issue of PG
 
  return returnSet;
};
