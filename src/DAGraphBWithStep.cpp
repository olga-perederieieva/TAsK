#include "DAGraphBWithStep.h"
#include "LineSearch.h"
#include "Path.h"
#include "StarNetwork.h"
#include "Derivative.h"
#include "StarLink.h"

#include <limits>
#include <cassert>

LineSearch* DAGraphBWithStep::lineSearch_ = NULL;

DAGraphBWithStep::DAGraphBWithStep(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol, 
					int originIndex, bool useMultiStep, LineSearch* lineSearch) : 
					DAGraphB(net, mat, zeroFlow, dirTol, originIndex, useMultiStep) {
	lineSearch_ = lineSearch;
};

DAGraphBWithStep::~DAGraphBWithStep(){

};

FPType DAGraphBWithStep::calcFlowStep(Path* minPath, Path* maxPath) const{
	int nbLinks = net_->getNbLinks();
	FPType x[nbLinks];
	FPType y[nbLinks];
	int indexes[nbLinks];
	
	for(StarLinkIterator it = maxPath->begin(); it != maxPath->end(); ++it){
		y[(*it)->getIndex()] = 0.0;
	}
	for (StarLinkIterator it = minPath->begin(); it != minPath->end(); ++it) {
		y[(*it)->getIndex()] = 0.0;
	}
	
	int linkIndex = -1;
	int size = 0;
	
	// projecting path direction onto links
	FPType descDirection = minPath->getCurrCost() - maxPath->getCurrCost(); 

	FPType oFlow = 0.0;
	FPType maxPathFlow = std::numeric_limits<FPType>::infinity( ); 
	for(StarLinkIterator it = maxPath->begin(); it != maxPath->end(); ++it){
		StarLink* link = *it;
		linkIndex = link->getIndex();
		x[linkIndex] = link->getFlow();
		y[linkIndex] += descDirection;	
		indexes[size] = linkIndex;
		++size;
		oFlow = getOriginFlow(linkIndex);
		if (oFlow < maxPathFlow) maxPathFlow = oFlow;
	}
	if (maxPathFlow == 0.0) {
		return 0.0;
	}

	for (StarLinkIterator it = minPath->begin(); it != minPath->end(); ++it) {
		StarLink* link = *it;
		linkIndex = link->getIndex();
		x[linkIndex] = link->getFlow();
		y[linkIndex] -= descDirection; 
		indexes[size] = linkIndex;
		++size;
	}
	
	(lineSearch_->getDerivative())->setDataPointers(size, x, y, indexes);
	
	// calculating upper bound for line search
	FPType ub = -maxPathFlow / descDirection;
	assert(ub > 0.0);
	return (lineSearch_->execute(0.0, ub) * -descDirection);
};