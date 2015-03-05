#include "DAGraphB.h"
#include "DAGraphNode.h"
#include "Path.h"

#include <cassert>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <limits>

FPType DAGraphB::dirTol_ = 0.0;
bool DAGraphB::useMultiStep_ = false;

DAGraphB::DAGraphB(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
				   int originIndex, bool useMultiStep) : 
				   DAGraph(net, mat, zeroFlow, originIndex), nbIter(0) {
	dirTol_ = dirTol;
	useMultiStep_ = useMultiStep;
};

DAGraphB::~DAGraphB(){

};

bool DAGraphB::moveFlow(){
	++nbIter;
	bool canMove = false;
	bool canMoveTmp = false;
	for (int i = beginDescPass(); i != -1; i = getNextDescPass()){
		canMoveTmp = performFlowMove(i, getOriginIndex()); 
		if (canMoveTmp) {
			canMove = true;
		}
	}
	return canMove;
}; 

bool DAGraphB::performFlowMove(int index, int origin){
	DAGraphNode * const node = getNode(index);
	if (node->maxDist - node->minDist  < dirTol_) {
		return false;
	}
	
	StarLink* minLink = node->minLink;
	StarLink* maxLink = node->maxLink;
	if (minLink == NULL || maxLink == NULL) {
		return false;
	}
	
	std::list<Path*> paths;
	Path minPath;
	Path maxPath;
	paths.push_back(&minPath); 
	paths.push_back(&maxPath); 
	
	FPType minDist = 0.0; 
	FPType maxDist = 0.0;

	int minNode = minLink->getNodeFromIndex();
	int maxNode = maxLink->getNodeFromIndex();
	if (minNode != maxNode) {
		minPath.addLinkToPath(minLink);
		minDist += minLink->getTime();
		maxPath.addLinkToPath(maxLink);
		maxDist += maxLink->getTime();
		
	} else if (minLink != maxLink) {
		minPath.addLinkToPath(minLink);
		minDist += minLink->getTime();
		maxPath.addLinkToPath(maxLink);
		maxDist += maxLink->getTime();
	}
	while (true) {
		if (minNode == maxNode) {
			break;
		} else {
			DAGraphNode * const nodeMin = getNode(minNode);
			DAGraphNode * const nodeMax = getNode(maxNode);
			if (nodeMin->post < nodeMax->post) { // minNode is upper in top order
				// we have to move minNode
				minLink = nodeMin->minLink;
				if (minLink != NULL) {
					minNode = minLink->getNodeFromIndex();
					minPath.addLinkToPath(minLink);
					minDist += minLink->getTime();
				}
			
			} else {
				maxLink = nodeMax->maxLink;
				if (maxLink != NULL) {
					maxNode = maxLink->getNodeFromIndex();
					maxPath.addLinkToPath(maxLink);
					maxDist += maxLink->getTime();
				}
			
			}
		}
		
	}
	
	bool canMoveFlow = false;
	if ( !minPath.isEmpty() && !maxPath.isEmpty() && ( maxDist - minDist > dirTol_) ) {
		assert(minNode == maxNode); 
		
		FPType dx = 0.0;	
		minPath.setCurrCost(minDist);
		maxPath.setCurrCost(maxDist);
		
		while (true) {
			dx = calcFlowStep(&minPath, &maxPath);	
			if (dx <= zeroFlow_) {
				break;
			} else {
				canMoveFlow = true;
			}
			
			minDist = 0.0; 
			for(StarLinkIterator it = minPath.begin(); it != minPath.end(); ++it){
				StarLink* link = *it;
				addFlow(link, dx);
				minDist += link->getTime();
			}
			
			maxDist = 0.0; 
			for(StarLinkIterator it = maxPath.begin(); it != maxPath.end(); ++it){
				StarLink* link = *it;
				addFlow(link, -dx);
				maxDist += link->getTime();
			}
			if (useMultiStep_ == false) break;

			if (maxDist <= minDist) {
				break;
			}
			
			minPath.setCurrCost(minDist); 
			maxPath.setCurrCost(maxDist); 
			
		}
	}
		
	return canMoveFlow;
};

void DAGraphB::printMinPathOFlows(int index) const{
	StarLink *link = getNode(index)->minLink;
	std::cout << "MIN PATH origin flows" << std::endl;
	while (link != NULL){
		std::cout << "[" << link->getNodeFromIndex() << ", " <<
				link->getNodeToIndex() << "] " << 
				getOriginFlow(link->getIndex()) << " ";
		link = getNode(link->getNodeFromIndex())->minLink;
	}
	std::cout << std::endl;
};

void DAGraphB::printMaxPathOFlows(int index) const{
	StarLink *link = getNode(index)->maxLink;
	std::cout << "MAX PATH origin flows" << std::endl;
	while (link != NULL){
		std::cout << "[" << link->getNodeFromIndex() << ", " <<
				link->getNodeToIndex() << "] " << 
				getOriginFlow(link->getIndex()) << " ";
		link = getNode(link->getNodeFromIndex())->maxLink;
	}
	std::cout << std::endl;	
};

void DAGraphB::addFlow(StarLink *link, FPType dFlow){
	link->addFlow(dFlow);
	int linkIndex = link->getIndex();
	addOriginFlow(linkIndex, dFlow);
	if (link->getFlow() <= zeroFlow_) {
		link->setFlow(0.0);
		setOriginFlowToZero(linkIndex);
	}
	if (getOriginFlow(linkIndex) < 0.0) {
		setOriginFlowToZero(linkIndex);
	}
	link->updateTime();
	
};

FPType DAGraphB::calcFlowStep(Path* minPath, Path* maxPath) const{
	FPType g = maxPath->getCurrCost() - minPath->getCurrCost();
	assert(g == g && g >= 0.0);
	if (g < dirTol_) return 0.0;
	
	FPType h = 0.0;
	for (StarLinkIterator it = minPath->begin(); it != minPath->end(); ++it) {
		h += (*it)->getDerivative();
	}
	FPType minMove = std::numeric_limits<FPType>::infinity( ); 
	FPType oFlow = 0.0;
	for(StarLinkIterator it = maxPath->begin(); it != maxPath->end(); ++it){
		StarLink* link = *it;
		h += link->getDerivative();
		oFlow = getOriginFlow(link->getIndex());
		if (oFlow < minMove) minMove = oFlow;
	}
	assert(h >= 0.0);
	FPType dx = g / h;
	if (dx > minMove) return minMove;
	return dx;
	
};
