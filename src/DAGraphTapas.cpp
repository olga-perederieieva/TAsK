#include "DAGraphTapas.h"
#include "Utils.h" 
#include "Error.h"
#include "PASManager.h"
#include "ShortestPath.h"
#include "PAS.h"

#include <cassert>
#include <math.h>

ShortestPath* DAGraphTapas::shPath_ = NULL;
PASManager* DAGraphTapas::pasManager_ = NULL;

DAGraphTapas::DAGraphTapas(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
							int originIndex, 
							ShortestPath *shPath, PASManager *pasManager) :
							DAGraph(net, mat, zeroFlow, originIndex), 
							minShift_(0.0), exploredLinks_() {
	 shPath_ = shPath;
	 pasManager_ = pasManager;
}; 

DAGraphTapas::~DAGraphTapas() {

}; 

bool DAGraphTapas::checkPositiveFlow(int linkIndex){
	return (getOriginFlow(linkIndex) > zeroFlow_);
};

bool DAGraphTapas::moveFlow() {
  shPath_->calculate(getOriginIndex()); 
  pasManager_->recalculatePASCosts();
  // find links that are used by the origin, but are not part of min cost tree
  std::list<StarLink*> inLinks;
  StarLink* link = NULL;
  StarLink* shPathLink = NULL;
  int i = -1;

  for (int j = 0; j < nodeSize_; ++j) { //iterating over nodes without topological sort
    i = nodeIndexes_[j];
    getInLinks(i, inLinks);
    shPathLink = shPath_->getInComeLink(i); 
    
    if (shPathLink != NULL) {
	    for (std::list<StarLink*>::iterator it = inLinks.begin();
	    		it != inLinks.end(); ++it){
	     	link = *it;
	      	// check if current link does not belong to the min-cost tree
		    if (getOriginFlow(link->getIndex()) > zeroFlow_ && link != shPathLink){
				pasManager_->createNewPAS(this, link, i);
		    }
		}
    }
  }

  // at the moment we just go through all active PAS
  bool flowWasMoved = false;
  for (CostPasIterator pasIt = pasManager_->begin(); pasIt != pasManager_->end(); ++pasIt){
	if ((*pasIt)->moveFlow()) {

		flowWasMoved = true;
	}
   }

  return flowWasMoved; 
}; 

void DAGraphTapas::setMinShift(FPType value){
	assert(value >= 0.0);
	minShift_ = value;
};

FPType DAGraphTapas::getMinShift(){
	return minShift_;
};


void DAGraphTapas::removeCyclicFlows(){
	while (true) {
		exploredLinks_.clear();
		if (topologicalSort() == false) break;
	};
};


// always returns true 
bool DAGraphTapas::handleBackEdge(StarLink* link){
	
	int linkInd = link->getIndex();
	int nextNode = link->getNodeFromIndex();
	int termNode = link->getNodeToIndex();
	StarLink* nextLink = NULL;
	std::list<StarLink*> cycle;
	cycle.push_back(link);
	FPType flow = 0.0;
	FPType minFlow = getOriginFlow(linkInd);
	for (std::list<StarLink*>::const_reverse_iterator it = exploredLinks_.rbegin();
			it != exploredLinks_.rend(); ++it) {
		nextLink = *it;
		if (nextLink->getNodeToIndex() == nextNode){
			flow = getOriginFlow(nextLink->getIndex());
			cycle.push_back(nextLink);
			if (flow < minFlow) minFlow = flow;
			nextNode = nextLink->getNodeFromIndex();
			if (nextNode == termNode) break;
		}
	}
	
	// reduce flow on cycle by minFlow
	StarLink *linkTmp = NULL;
	int linkTmpIndex = -1;
	for(std::list<StarLink*>::iterator it = cycle.begin(); it != cycle.end(); ++it) {
		linkTmp = *it;
		linkTmpIndex = linkTmp->getIndex();
		if (getOriginFlow(linkTmpIndex) - minFlow < zeroFlow_) {
			setOriginFlowToZero(linkTmpIndex);
		} else {
			addOriginFlow(linkTmpIndex, -minFlow);
		}
		if (linkTmp->getFlow() - minFlow < zeroFlow_) {
			linkTmp->setFlowToZero();
		} else {
			linkTmp->addFlow(-minFlow);
		}
		linkTmp->updateTime();
	}
	return true;	
};

void DAGraphTapas::handleExploredLink(StarLink* link){
	exploredLinks_.push_back(link);
};

