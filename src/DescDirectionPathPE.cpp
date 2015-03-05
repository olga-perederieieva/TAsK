#include "DescDirectionPathPE.h"

#include <cassert>

 DescDirectionPathPE::DescDirectionPathPE(FPType delta) : DescDirectionPath(delta){
 
 };
 
 DescDirectionPathPE::~DescDirectionPathPE(){
  
 };
  
 PathAndDirection** DescDirectionPathPE::createPathDirection(int &size, const std::list<Path*> &paths, 
                                              bool &isEquilibrated){
  	
  	// find sh path and longest path
  	Path *minPath = findMinPath(paths);
  	FPType minDist = minPath->getCurrCost();
  	Path *maxPath = findMaxPath(paths, minPath);
  	FPType maxDist = maxPath->getCurrCost();
  	
  	// calculate direction
  	FPType dir = minDist - maxDist;
  	if (-dir <= delta_) {
  		isEquilibrated = true;
  	  size = 0;
		  return NULL;
  	}

    isEquilibrated = false;
    size = 2;
    PathAndDirection **returnSet = new PathAndDirection*[size];
    for (int i = 0; i < size; ++i) {
      returnSet[i] = new PathAndDirection();
    }

  	(returnSet[0])->setPath(maxPath);
  	(returnSet[0])->setDirection(dir);
  	(returnSet[1])->setPath(minPath);
  	(returnSet[1])->setDirection(-dir);
  	return returnSet;
 };
