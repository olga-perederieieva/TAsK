#include "OriginBush.h"
#include "PairOD.h"
#include "StarLink.h"
#include "DAGraph.h"

#include <stdlib.h>
#include <iostream>
#include <cassert>

void OriginBush::allocateDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol){
	assert(daGraph_ == NULL);
	daGraph_ = createNewDAG(index, net, mat, zeroFlow, dirTol); 
};

OriginBush::OriginBush() : daGraph_(NULL), topSortUpToDate_(false) { 
};

OriginBush::~OriginBush(){
	delete daGraph_;
};

void OriginBush::updateTopSort(){
	// call topological sort if some of the links were removed on previous call of 
	// mainLoop on this bush
	if (!topSortUpToDate_) {
		daGraph_->topologicalSort();
		topSortUpToDate_ = true;
	}
};

bool OriginBush::improve(){
	
	assert(topSortUpToDate_ == true);
	// 1. calculate min- and max-trees - initialise u_i and U_i (it is assumed that 
	// topological order exists)
	daGraph_->buildMinMaxTrees(-1);
	
	// 2. traverse all links that are not in the set yet and check if it is worth adding
	bool wasImproved = daGraph_->addBetterLinks();
	
	// 3. if the bush was improved - topological sort - to build passes
	if (wasImproved) { 
		daGraph_->topologicalSort();
		callBuildMinMaxTrees();
	}
	return wasImproved;
};

void OriginBush::callBuildMinMaxTrees(){
	daGraph_->buildMinMaxTrees(-1);
};

bool OriginBush::equilibrate(){
	return daGraph_->moveFlow();
};

void OriginBush::removeUnusedLinks(){
	// remove links with zero flow from daGraph_ AND maintain connectivity
	if (daGraph_->removeUnusedLinks()) {
		topSortUpToDate_ = false;
	}
};

void OriginBush::addLinkAndUpdateFlow(StarLink *link, PairOD* dest){
	assert(link != NULL && dest != NULL);
	daGraph_->addLink(link);
	if (dest->getODIndex() != -1) {
		FPType demand = dest->getDemand();
		daGraph_->addOriginFlow(link->getIndex(), demand);
		link->addFlow(demand);
	}
};

void OriginBush::print(){
	std::cout << "Origin: " << daGraph_->getOriginIndex() << " daGraph_ = " << 
				daGraph_ << std::endl;
	daGraph_->print();
};

int OriginBush::getOriginIndex() const{
	return daGraph_->getOriginIndex();
};

void OriginBush::printOriginFlow() const{
	daGraph_->printOriginFlow();
};

FPType OriginBush::checkOFlowsFeasibility(){
	return daGraph_->checkOFlowsFeasibility();
}; 
