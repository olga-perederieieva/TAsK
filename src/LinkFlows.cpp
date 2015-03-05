#include "LinkFlows.h"
#include "StarNetwork.h"
#include "LabelCorrectingAl.h"
#include "LineSearch.h"
#include "Derivative.h"

#include <stdlib.h>
#include <iostream>

LinkFlows::LinkFlows(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath,
			LineSearch* lineSearch, FPType zeroFlow,
			LabelCorrectingAl* LCShPath) :
			nbLinks_(net->getNbLinks()), stepPrev_(0.0), stepPrevPrev_(0.0), zeroFlow_(zeroFlow), 
			lineSearch_(lineSearch), net_(net), aon_(*mat, LCShPath), minTravelTime_(0.0) {
	
	linkFlows_ = new FPType[nbLinks_];
	linkFlowsAux_ = new FPType[nbLinks_];
	indexes_ = new int[nbLinks_];
	direction_ = new FPType[nbLinks_];
	for (int i = 0; i < nbLinks_; ++i) {
		linkFlows_[i] = 0.0;
		linkFlowsAux_[i] = 0.0;
		indexes_[i] = i;
		direction_[i] = 0.0;
	}
	
};

LinkFlows::~LinkFlows(){
	delete[] linkFlows_;
	delete[] linkFlowsAux_;
	delete[] indexes_;
	delete[] direction_;
};

FPType LinkFlows::getMinTravelTime() const{
	return minTravelTime_;
};

void LinkFlows::initialise(){
	minTravelTime_ = aon_.execute(this);
	for(int i = 0; i < nbLinks_; ++i) {
		linkFlows_[i] = linkFlowsAux_[i];
	}
};

void LinkFlows::updateLinkFlows(){
	for (StarLink *link = net_->beginOnlyLink(); link != NULL; link = net_->getNextOnlyLink()) {
		link->setFlow(linkFlows_[link->getIndex()]);
		link->updateTime();
	}
};

void LinkFlows::calculateFWAux(){
	for(int i = 0; i < nbLinks_; ++i) {
		linkFlowsAux_[i] = 0.0;
	}
	minTravelTime_ = aon_.execute(this);
};

void LinkFlows::calculateDirection(){
	calculateFWAux();
	for(int i = 0; i < nbLinks_; ++i) {
		direction_[i] = linkFlowsAux_[i] - linkFlows_[i];	
	}
};
		
void LinkFlows::equilibrate(){
	
	calculateDirection();
	
	stepPrevPrev_ = stepPrev_;
	(lineSearch_->getDerivative())->setDataPointers(nbLinks_, linkFlows_, direction_, indexes_);
	stepPrev_ = lineSearch_->execute(0.0, 1.0);
	for(int i = 0; i < nbLinks_; ++i) {
		linkFlows_[i] += stepPrev_ *  direction_[i]; 
		if (linkFlows_[i] < zeroFlow_) {
			linkFlows_[i] = 0.0;
		}
	}
};

void LinkFlows::initialiseItself(StarLink* link, PairOD *dest, int originIndex){
	linkFlowsAux_[link->getIndex()] += dest->getDemand();
};
