#include "PAS.h"
#include "DAGraphTapas.h"
#include "Utils.h"
#include "StarLink.h"

#include <cassert>
#include <limits> 
#include <iostream>

FPType PAS::zeroFlow_ = 0.0;
FPType PAS::dirTol_ = 0.0;

PAS::PAS(FPType zeroFlow, FPType dirTol) : cheapCost_(0.0), expCost_(0.0), totalShift_(0.0),
 							cheapSegm_(0), relevantOrigins_(), nbFlowMoves_(0) {  
	zeroFlow_ = zeroFlow;
	dirTol_ = dirTol;
}; 

PAS::~PAS() {

}; 

bool PAS::isUnused() {
	int retVal = nbFlowMoves_ ;
	nbFlowMoves_ = 0;
	return retVal < 0;
}; 

void PAS::pushFrontToCheap(StarLink *link) {
	segments_[cheapSegm_].push_front(link);
	cheapCost_ += link->getTime();
}; 

void PAS::pushFrontToExp(StarLink *link) {
	segments_[1 - cheapSegm_].push_front(link);
	expCost_ += link->getTime();
}; 

void PAS::pushBackToCheap(StarLink *link){
	segments_[cheapSegm_].push_back(link);
	cheapCost_ += link->getTime();
};

void PAS::pushBackToExp(StarLink *link){
	segments_[1 - cheapSegm_].push_back(link);
	expCost_ += link->getTime();
};		
		
StarLink* PAS::getLastCheapLink(){
	if (segments_[cheapSegm_].empty()) return NULL;
	return segments_[cheapSegm_].back();
};

StarLink* PAS::getLastExpLink(){
	if (segments_[1 - cheapSegm_].empty()) return NULL;
	return segments_[1 - cheapSegm_].back();
};
				
bool PAS::addOrigin(DAGraphTapas* dag) {
	assert(dag != NULL);
	return relevantOrigins_.insert(dag).second;
}; 

bool PAS::checkIfEffective(FPType cost, FPType v, int index, DAGraphTapas* dag){
	return checkIfCostEffective(cost) && checkIfFlowEffective(v, index, dag);
};

bool PAS::checkIfFlowEffective(FPType v, int index, DAGraphTapas* dag) const{
	assert(index >= 0);
	FPType minFlow = std::numeric_limits<FPType>::infinity();
	FPType flow = 0.0;
	int expIndex = 1 - cheapSegm_;
	for (std::list<StarLink*>::const_iterator it = segments_[expIndex].begin(); 
						it != segments_[expIndex].end(); ++it) {
		flow = dag->getOriginFlow((*it)->getIndex());
		if (flow < minFlow) minFlow = flow; 
	}
	return (minFlow >= v * dag->getOriginFlow(index));
};

bool PAS::checkIfCostEffective(FPType cost) const{
	return (expCost_ - cheapCost_ >= cost);
};

bool PAS::moveFlow() {
	
	--nbFlowMoves_;
	bool tmp = true;
	if (recalcPASCosts() >= dirTol_) {
		
		FPType dFlow = calculateFlowShift(); // calculates appropriate shift and min shifts
											 // for each origin
		
		StarLink* link = NULL;
		int expIndex = 1 - cheapSegm_;
		// update origin flows
		DAGraphTapas* dagPointer = NULL;
		for (SetType::iterator dag = relevantOrigins_.begin(); dag != relevantOrigins_.end(); ++dag) {
			if (totalShift_ > 0.0) {
				dagPointer = *dag;
				FPType flowShift = dagPointer->getMinShift() / totalShift_ * dFlow;
				if (flowShift > zeroFlow_) {
					tmp = false;
					for (std::list<StarLink*>::iterator it = segments_[cheapSegm_].begin(); 
								it != segments_[cheapSegm_].end(); ++it) {
						dagPointer->addOriginFlowAndCreateLink(*it, flowShift); // link will be added 
																				// to the bush if 
																				// it is not there
						
					}
					
					for (std::list<StarLink*>::iterator it = segments_[expIndex].begin(); 
									it != segments_[expIndex].end(); ++it) {
						link = *it;
						int index = link->getIndex();
						if (dagPointer->getOriginFlow(index) - flowShift < zeroFlow_) {
							dagPointer->setOriginFlowToZero(index);
						} else {
							dagPointer->addOriginFlowAndCreateLink(link, -flowShift); // link will be 
																					  // added to the bush 
																					  // if it is not there
						}
						
					}
				}
			} else {
				assert(dFlow == 0.0);
			}
		}
		
		// update link flows and costs
		if (tmp == false) {
			for (std::list<StarLink*>::iterator it = segments_[cheapSegm_].begin(); it != segments_[cheapSegm_].end(); 
														++it) {
				link = *it;
				link->addFlow(dFlow);
				link->updateTime();
			}
			for (std::list<StarLink*>::iterator it = segments_[expIndex].begin(); it != segments_[expIndex].end(); 
														++it) {
				link = *it;
				if (link->getFlow() - dFlow < zeroFlow_) {
					link->setFlowToZero();
				} else {
					link->addFlow(-dFlow);
				}
				link->updateTime();

			}
			++nbFlowMoves_;
			return true;
		} 
		
	}
	return false;
}; 

//
FPType PAS::calculateFlowShift(){
	// calculate maximum feasible shift
	totalShift_ = 0.0;
	DAGraphTapas* dagPointer = NULL;
	for (SetType::iterator dag = relevantOrigins_.begin(); dag != relevantOrigins_.end(); ++dag) {
		FPType minFlowShift = std::numeric_limits<FPType>::infinity();
		dagPointer = *dag;
		FPType oFlow = 0.0;
		for (std::list<StarLink*>::iterator it = segments_[1 - cheapSegm_].begin(); 
												it != segments_[1 - cheapSegm_].end(); ++it) {
			
			oFlow = dagPointer->getOriginFlow((*it)->getIndex());
			if (oFlow < minFlowShift) minFlowShift = oFlow;
		}
		dagPointer->setMinShift(minFlowShift);
		totalShift_ += minFlowShift;
	}
	
	FPType dFlow = getFlowShift();
	assert(dFlow >= 0.0 && dFlow == dFlow);
	if (dFlow > totalShift_) {
		dFlow = totalShift_;
	}
	return dFlow;
};

FPType PAS::getFlowShift(){
	return (expCost_ - cheapCost_) / Utils::calculatePathDerivativeForDisjoint(segments_[cheapSegm_],
								segments_[1 - cheapSegm_]);
};

FPType PAS::calcSegCost(int index){
	FPType segCost = 0.0;
	for (std::list<StarLink*>::iterator it = segments_[index].begin(); it != segments_[index].end(); ++it) {
		segCost += (*it)->getTime();
	}
	return segCost;
};

FPType PAS::getCostDiff(){
	return expCost_ - cheapCost_;
};

FPType PAS::recalcPASCosts(){
	FPType cost0 = 	calcSegCost(0);
	FPType cost1 = 	calcSegCost(1);
	if (cost0 < cost1) {
		cheapSegm_ = 0;
		cheapCost_ = cost0;
		expCost_ = cost1;
	} else {
		cheapSegm_ = 1;
		cheapCost_ = cost1;
		expCost_ = cost0;
	}
	return expCost_ - cheapCost_;
};

void PAS::print()const  {
	StarLink *link = NULL;
	std::cout << "cheap segment cost = " << cheapCost_ << " ";
	for(std::list<StarLink*>::const_iterator it = segments_[cheapSegm_].begin(); it != segments_[cheapSegm_].end(); ++it){
		link = *it;
		std::cout << "link " << link->getIndex() << " [" << link->getNodeFromIndex() << " ," << 
			link->getNodeToIndex() << "] time = " << link->getTime() << " flow = " << link->getFlow() << " ";
	}
	std::cout << std::endl; 
	std::cout << "expensive segment cost = " << expCost_ << " ";
	for(std::list<StarLink*>::const_iterator it = segments_[1 - cheapSegm_].begin(); 
										it != segments_[1 - cheapSegm_].end(); ++it){
		link = *it;
		std::cout << "link " << link->getIndex() << " [" << link->getNodeFromIndex() << " ," << 
			link->getNodeToIndex() << "] time = " << link->getTime() << " flow = " << link->getFlow() << " ";
	}
	std::cout << std::endl;
	std::cout << "Origins: ";
	for(SetType::const_iterator it = relevantOrigins_.begin(); it != relevantOrigins_.end(); ++it){
		std::cout << (*it)->getOriginIndex() << " ";
	}
	std::cout << std::endl;
}; 

int PAS::getNbLinks() const{
	return segments_[0].size() + segments_[1].size();
};

int PAS::getNbOrigins() const{
	return relevantOrigins_.size();
};

