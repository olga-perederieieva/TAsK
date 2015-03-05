#include "ODSet.h"
#include "DescDirectionPathPE.h"
#include "DescDirectionPathGP.h"
#include "DescDirectionPathPG.h"
#include "Error.h"
#include "Utils.h"
#include "PathBasedFlowMovePE.h"
#include "PathBasedFlowMoveGP.h"
#include "PathCost.h"
#include "ShortestPath.h"

#include <stdlib.h>
#include <iostream>
#include <limits>
#include <math.h>
#include <vector>
#include <iomanip> 

StarNetwork *ODSet::net_ = NULL;
ShortestPath *ODSet::shPath_ = NULL;
FPType ODSet::zeroFlow_ = 0.0;
PathCost *ODSet::pathCost_ = NULL;
PathBasedFlowMove *ODSet::flowMove_ = NULL;

		
ODSet::ODSet(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net, 
		ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMove* flowMove) : 
		 destIndex_(destIndex), originIndex_(originIndex), index_(index){
	net_ = net;
	shPath_ = shPath;
	
	pathCost_ = pathCost;
	zeroFlow_ = zeroFlow;
	
	minDist_ = std::numeric_limits<FPType>::infinity( );
	
	flowMove_ = flowMove;
};


ODSet::~ODSet(){
	for (std::list<Path*>::iterator it = allPaths_.begin(); it != allPaths_.end(); ++it) {
		delete *it;
	}
};

int ODSet::getIndex() const{
	return index_;
};

int ODSet::getDestIndex() const{
	return destIndex_;
};

int ODSet::getOriginIndex() const{
	return originIndex_;
};

void ODSet::addPath(Path *path){
	allPaths_.push_back(path);
	FPType pathCost = path->getCurrCost();
	if (pathCost < minDist_) minDist_ = pathCost;
};
		
PathIterator ODSet::begin() const{
	return allPaths_.begin();
};

PathIterator ODSet::end() const{
	return allPaths_.end();
};

void ODSet::print() const{
	std::cout << "OD pair# " << getIndex() << " origin = " << getOriginIndex() << " dest = " 
		<< getDestIndex()  << std::endl; 
	for (PathIterator pathIt = begin(); pathIt != end(); ++pathIt) {
		(*pathIt)->print();
	}
	std::cout << std::endl;
};

void ODSet::updateSet(){
	
	FPType dist = 0.0;
	minDist_ = std::numeric_limits<FPType>::infinity( );
	for (PathIterator pathIt = begin(); pathIt != end(); ++pathIt) {
		Path* path = *pathIt;
		dist = pathCost_->calculate(path, index_);
		path->setCurrCost(dist);
		if (dist < minDist_) {
			minDist_ = dist;
		}
	}
};

bool ODSet::improveSet(){
	shPath_->calculate(originIndex_, destIndex_, index_);
	FPType minDist = shPath_->getCost(destIndex_);
	if (minDist - minDist_ < -zeroFlow_) { // if we add link costs in different order path 
											// costs might not be the same
		Path *path = new Path();
		path->setCurrCost(minDist);
		StarLink *link = shPath_->getInComeLink(destIndex_);
		int nextDest = link->getNodeFromIndex();
		while (link != NULL) {
			path->addLinkToPath(link);
			nextDest = link->getNodeFromIndex();
			link = shPath_->getInComeLink(nextDest);
		}
		addPath(path);
		return true;
	}
	return false;
};

void ODSet::setPrevFlows(){
	for (PathIterator pathIt = begin(); pathIt != end(); ++pathIt) {
		Path* path = *pathIt;
		path->setPrevFlow(path->getFlow());
	}
};

bool ODSet::equilibrate(){
	//save current flow to prev flow
	setPrevFlows();
	
	bool isEquilibrated = false;
	isEquilibrated = flowMove_->calculatePathDirection(allPaths_);
	if (isEquilibrated) {
		return true;
	}
	
	calculateProjectedDirection(); // hook method for children classes
	isEquilibrated = flowMove_->executeFlowMove(); 
	
	if (isEquilibrated) {
		return true;
	}
	
	return false;
};

int ODSet::getNbPaths() const{
	return allPaths_.size(); 
};
		
void ODSet::removeUnusedPaths(){
	Path* path = NULL;
	for (std::list<Path*>::iterator it = allPaths_.begin(); it != allPaths_.end(); ++it) {
		path = *it;
		if (path->getFlow() <= zeroFlow_) {
			delete path;
			it = allPaths_.erase(it);
			--it;
		}
	}
};


void ODSet::projectPathFlowOnLinks(){
	
	FPType flow = 0.0;
	for (PathIterator pathIt = begin(); pathIt != end(); ++pathIt) {
		Path* path = *pathIt;
		flow = path->getFlow() - path->getPrevFlow();
		if (fabs(flow) > zeroFlow_) {
			for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
				(*it)->addFlow(flow);
			}
		}
	}
	for (PathIterator pathIt = begin(); pathIt != end(); ++pathIt) {
		Path* path = *pathIt;
		for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
			StarLink* link = *it;
			if (link->getFlow() < zeroFlow_) {
					link->setFlow(0.0);
			}
			link->updateTime();
		}
	}

};

FPType ODSet::getMinPathCost() const {
	return minDist_;
};