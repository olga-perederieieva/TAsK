#include "PASManager.h"
#include "DAGraphTapas.h"
#include "PAS.h"
#include "ShortestPath.h"

#include <cassert>
#include <math.h>
#include <iostream>
#include <stdlib.h>

PASManager::PASManager(const ShortestPath &shPath, FPType dirTol, int nbNodes, FPType mu, FPType v, 
						FPType zeroFlow) : 
						zeroFlow_(zeroFlow), dirTol_(dirTol), shPath_(shPath), 
						listOfPas_(),  nbNodes_(nbNodes), 
						mu_(mu), v_(v), setOfNodes_(), nbIter_(1) {
	srand (time(NULL));

}; 

PASManager::~PASManager() {
	for (std::list<PAS*>::iterator it = listOfPas_.begin(); it != listOfPas_.end(); ++it){
		delete *it;
	}
}; 

void PASManager::recalculatePASCosts(){
	for (CostPasIterator pasIt = begin(); pasIt != end(); ++pasIt){
		(*pasIt)->recalcPASCosts();
	}
};

PAS* PASManager::pasExist(StarLink* cheapLink, StarLink* expLink){
	assert(cheapLink != NULL && expLink != NULL);
	for (CostPasIterator pasIt = begin(); pasIt != end(); ++pasIt){
		PAS* pas = *pasIt;
		if (pas->getLastExpLink() == expLink && pas->getLastCheapLink() == cheapLink) {
			return pas;
		}
	}
	return NULL;
}

bool PASManager::nodeBelongsToShPath(int nodeIndex){
	return (setOfNodes_.find(nodeIndex) != setOfNodes_.end());
};

void PASManager::createExpSegment(PAS *pas, const std::vector<StarLink*> &checkedLinks,
								  int divergeNode, int mergingNodeIndex){
	int size = checkedLinks.size();
	assert(divergeNode >= 0 && divergeNode <  size && mergingNodeIndex >= 0 &&
		   mergingNodeIndex < size);
	int start = divergeNode;
	StarLink* link = NULL;
	while (start != mergingNodeIndex){
		link = checkedLinks[start];
		pas->pushBackToExp(link);
		start = link->getNodeToIndex();
	}
};

void PASManager::createCheapSegment(PAS *pas, int divergeNode, int mergingNodeIndex){
	StarLink *link = shPath_.getInComeLink(mergingNodeIndex);
	int nextDest = link->getNodeFromIndex();
	while (link != NULL) {
		pas->pushFrontToCheap(link);
		nextDest = link->getNodeFromIndex();
		if (nextDest == divergeNode) return;
		link = shPath_.getInComeLink(nextDest);
	}
};

void PASManager::prepareShPathStructure(int mergingNodeIndex){
 	setOfNodes_.clear();
 	StarLink *link = shPath_.getInComeLink(mergingNodeIndex);
	int nextDest = link->getNodeFromIndex();
	while (true) {
		setOfNodes_.insert(nextDest);
		link = shPath_.getInComeLink(nextDest);
		if (link == NULL) break;
		nextDest = link->getNodeFromIndex();
	}
};

PAS* PASManager::allocatePAS(){
	return new PAS(zeroFlow_, dirTol_); 
};

PAS* PASManager::createPAS(DAGraphTapas *dag, StarLink *expLink, int mergingNodeIndex, 
							bool (*checkPtr)(FPType originFlow, FPType thr)){
	// data for BFS
	std::list<std::pair<int, StarLink*> > bfsQueue;
	bfsQueue.push_back(std::make_pair(expLink->getNodeFromIndex(), expLink));
	std::vector<int> nodeState(nbNodes_, -1); // -1 - node was not investigated
	std::vector<StarLink*> checkedLinks(nbNodes_);
	for (int i = 0; i < nbNodes_; ++i)
		checkedLinks[i] = NULL;
	
	prepareShPathStructure(mergingNodeIndex);
	std::list<StarLink*> inLinks;
	StarLink* link = NULL;
	int divergeNode = -1;
	FPType thr = v_ * dag->getOriginFlow(expLink->getIndex());
	bool pasCreated = true;
	bool canStop = false;
	while (!canStop) {
		// pop the first element from queue for investigation
		if (bfsQueue.empty()) {
			pasCreated = false; // sometimes it is not possible to create a PAS
			break;
		}
		std::pair<int, StarLink*> firstInQueue(bfsQueue.front());
	
		// copy poped element to checkedLinks
		checkedLinks[firstInQueue.first] = firstInQueue.second;
		bfsQueue.pop_front();
			
		// check if we can stop
		if (nodeBelongsToShPath(firstInQueue.first)) {
			divergeNode = firstInQueue.first;
			break;
		}
		
		// add new nodes from used links to the queue 
		dag->getInLinks(firstInQueue.first, inLinks);
		for (std::list<StarLink*>::iterator it = inLinks.begin(); it != inLinks.end(); ++it){
     		link = *it;
     		FPType originFlow = dag->getOriginFlow(link->getIndex());
     		if (checkPtr(originFlow, thr)) { 
	     		int nodeFromIndex = link->getNodeFromIndex();
	     		if (originFlow > zeroFlow_ && nodeState[nodeFromIndex] == -1) {
					bfsQueue.push_back(std::make_pair(nodeFromIndex, link));
					// update  node state
					nodeState[nodeFromIndex] = 1; // node is in the queue
					// copy poped element to checkedLinks
					checkedLinks[nodeFromIndex] = link;
				
					// check if we can stop
					if (nodeBelongsToShPath(nodeFromIndex)) { 
						// this check is redundant. TODO: think how to make it better
						divergeNode = nodeFromIndex;
						canStop = true;
						break;
					}
				}
			}
		}
		
		// update  node state
		nodeState[firstInQueue.first] = 0; // node is visited
	
	}
	
	if (pasCreated){	
		PAS* pas = allocatePAS(); 
	 	createExpSegment(pas, checkedLinks, divergeNode, mergingNodeIndex);
		createCheapSegment(pas, divergeNode, mergingNodeIndex);
		if (pas->getCostDiff() < dirTol_) {
			delete pas;
			pas = NULL;
		} else {

			pas->addOrigin(dag);
			listOfPas_.push_back(pas);

		}
		return pas;
		
	}

	return NULL;
};

FPType PASManager::calculateReducedCost(StarLink *expLink){
	return shPath_.getCost(expLink->getNodeFromIndex()) + expLink->getTime()
						   - shPath_.getCost(expLink->getNodeToIndex());
};

FPType PASManager::calcThreshold(){
	return 10.0 * pow(10, -nbIter_); 
}; 

bool checkThreshold(FPType originFlow, FPType thr) {return (originFlow >= thr);};
		
bool checkThresholdFake(FPType originFlow, FPType thr) {return true;};
		
void PASManager::createNewPAS(DAGraphTapas *dag, StarLink *expLink, int mergingNodeIndex) {
	PAS* foundPAS = pasExist(shPath_.getInComeLink(mergingNodeIndex), expLink);
	bool isEffective = false;
	FPType reducedCost = calculateReducedCost(expLink);
	int expIndex = expLink->getIndex();
	bool (*fncPtr)(FPType originFlow, FPType thr);
	FPType redVal = mu_ * reducedCost;
	if (foundPAS != NULL) {
		foundPAS->addOrigin(dag);
		isEffective = foundPAS->checkIfEffective(redVal, v_, expIndex, dag);
	} else { //we need to create a new PAS and add it to listOfPas_
		fncPtr = &checkThresholdFake;	
		PAS* pasTmp = createPAS(dag, expLink, mergingNodeIndex, fncPtr);
		if (pasTmp != NULL) {
			isEffective = pasTmp->checkIfEffective(redVal, v_, expIndex, dag); 
		} 
	}
		
	if (reducedCost > calcThreshold() && !isEffective) { // checks if link expLink is a 
														 // problematic OLC
		fncPtr = &checkThreshold;
		createPAS(dag, expLink, mergingNodeIndex, fncPtr);
	}
}; 


void PASManager::deleteUnusedPASAndMoveFlow(){
	++nbIter_; // this method is called after each loop -> we can calculate the number 
			   // of iterations here.
	PAS* pas = NULL;
	
	for (std::list<PAS*>::iterator it = listOfPas_.begin(); it != listOfPas_.end(); ++it){
		pas = *it;
		pas->moveFlow();
		if (pas->isUnused()) {
			delete *it;
			it = listOfPas_.erase(it);
			--it;
		}	
	}
}; 

CostPasIterator PASManager::begin() const {
	return listOfPas_.begin();
}; 

CostPasIterator PASManager::end() const {
	return listOfPas_.end();
}; 
		
int PASManager::getNbPAS()const  {
	return listOfPas_.size();
}; 

void PASManager::print()const  {
	for(CostPasIterator pasIt = begin(); pasIt != end(); ++pasIt){
		(*pasIt)->print();
	}
}; 
