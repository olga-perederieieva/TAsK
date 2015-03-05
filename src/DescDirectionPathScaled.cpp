#include "DescDirectionPathScaled.h"
#include "Utils.h"

DescDirectionPathScaled::DescDirectionPathScaled(DescDirectionPath *pathDir, int nbLinks,
											FPType delta): 
											DescDirectionPath(delta), pathDir_(pathDir),
											nbLinks_(nbLinks){

};

DescDirectionPathScaled::~DescDirectionPathScaled(){
	delete pathDir_;
};

PathAndDirection** DescDirectionPathScaled::createPathDirection(int &size, const std::list<Path*>
										&paths, bool &isEquilibrated){
	PathAndDirection** returnVal = pathDir_->createPathDirection(size, paths, isEquilibrated);
	if (isEquilibrated) {
		return NULL;
	}
	// It is assumed that positive value of direction is always the last one!!!
	FPType positiveDir = 0.0;
	FPType currDirection = 0.0;
	FPType currDer = 0.0;
	Path *minPath = (returnVal[size-1])->getPath();
	for (int i = 0; i < size-1; ++i) {
		currDer = Utils::calculatePathDerivative((returnVal[i])->getPath(), minPath, nbLinks_);
		currDirection = (returnVal[i])->getDirectionValue() / currDer;
		(returnVal[i])->setDirection(currDirection);
		positiveDir -= currDirection;
	}
	(returnVal[size-1])->setDirection(positiveDir);
	return returnVal;
};
