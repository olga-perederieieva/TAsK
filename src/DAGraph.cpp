#include "DAGraph.h"
#include "Error.h"
#include "StarNetwork.h"
#include "DAGraphNode.h"
#include "ODMatrix.h"
#include "Origin.h"
#include "PairOD.h"
#include "StarNode.h"

#include <cassert>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <limits>

StarNetwork* DAGraph::net_ = NULL;
ODMatrix* DAGraph::mat_ = NULL;
bool DAGraph::wasInitialised_ = false;
FPType DAGraph::zeroFlow_ = 0.0;

DAGraph::DAGraph(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, int originIndex) : nodeSize_(0), 
			originIndex_(originIndex),   linkSize_(0), clock_(1) {
	
	if (!wasInitialised_) initialiseStaticMembers(net, mat, zeroFlow);
	wasInitialised_ = true;
	
	int nbNodes = net->getNbNodes();
	
	nodes_ = new DAGraphNode*[nbNodes]; 
	nodeIndexes_ = new int[nbNodes];
	for (int i = 0; i < nbNodes; ++i) {
		nodes_[i] = NULL;
		nodeIndexes_[i] = -1;
	}
	
	int nbLinks = net->getNbLinks();
	originFlows_ = new FPType[nbLinks];
	links_ = new StarLink*[nbLinks];
	
	for (int i = 0; i < nbLinks; ++i) {
		links_[i] = NULL;
		originFlows_[i] = 0.0;
	}
	
	currNode_ = topOrder_.begin();
	currNodeDesc_ = topOrder_.rbegin();
	
};

DAGraph::~DAGraph(){
	for (int i = 0; i < nodeSize_; ++i) {
		delete nodes_[nodeIndexes_[i]];
	}
	delete[] nodes_;
	delete[] nodeIndexes_;
	delete[] links_;
	delete[] originFlows_;
};

void DAGraph::initialiseStaticMembers(StarNetwork *net, ODMatrix *mat, FPType zeroFlow){
	net_ = net;
	mat_ = mat;
	zeroFlow_ = zeroFlow;
};

FPType DAGraph::getDemand(int nodeIndex) const{
	return (nodes_[nodeIndex])->demand;
};

void DAGraph::setOriginFlowToZero(int linkIndex){
	originFlows_[linkIndex] = 0.0;
};

int DAGraph::getOriginIndex() const{
	return originIndex_;
};

void DAGraph::addOriginFlowAndCreateLink(StarLink* link, FPType demand){
	assert(link != NULL);
	int index = link->getIndex();
	if (links_[index] == NULL) {
		addLink(link);
	}
	addOriginFlow(index, demand);
};

void DAGraph::addOriginFlow(int linkIndex, FPType demand){
	assert(linkIndex >= 0 && linkIndex < net_->getNbLinks() && links_[linkIndex] != NULL);
	originFlows_[linkIndex] += demand;
};


bool DAGraph::handleBackEdge(StarLink* link){
	std::cout << "Back egde detected in DAG Link = " <<  link->getIndex() << " oflow = " << 
		getOriginFlow(link->getIndex()) << link->toString() << std::endl;
	throw Error("Back egde detected in DAG"); 
	return true;
};

bool DAGraph::explore(int vertex, bool *visited){
	
	visited[vertex] = true;
	preVisit(vertex);
	std::list<StarLink*> &linksList = (nodes_[vertex])->outLinks;
	int index = -1;
	bool backEdgeDetected = false;
	StarLink* link = NULL;
	for(std::list<StarLink*>::iterator it = linksList.begin(); it != linksList.end(); ++it){
		link = *it;
		index = link->getNodeToIndex();
		if (checkPositiveFlow(link->getIndex())) {
			handleExploredLink(link);
			if ((nodes_[index])->pre == 0) {
				backEdgeDetected = explore(index, visited);
				if (backEdgeDetected) return true;
			}
			if ((nodes_[index])->pre > 0 && (nodes_[index])->post == 0) {
				return handleBackEdge(link);
			}
		} 
	}
	postVisit(vertex);
	return false;
};

void DAGraph::preVisit(int vertex){
	(nodes_[vertex])->pre = clock_;
	++clock_;
};

void DAGraph::postVisit(int vertex){
	(nodes_[vertex])->post = clock_;
	++clock_;
	topOrder_.push_front(vertex);
};
		
bool DAGraph::topologicalSort(){

	clock_ = 1;
	topOrder_.clear();
	int nbNodes = net_->getNbNodes(); 
	bool visited[nbNodes];
	for (int i = 0; i < nbNodes; ++i) {
		visited[i] = false;
	}
	int index = -1;
	for (int i = 0; i < nodeSize_; ++i) {
		index = nodeIndexes_[i];
		(nodes_[index])->pre = 0;
		(nodes_[index])->post = 0;
	}
	
	
	for (int i = 0; i < nodeSize_; ++i) {
		index = nodeIndexes_[i];
		if (!visited[index]) {
			bool tmp = explore(index, visited);
			if (tmp == true) {
				return true;
			}
		}
	}
	return false;
	
};

void DAGraph::buildMinMaxTrees(int destIndex){
	int index = -1;

	for (int i = 0; i < nodeSize_; ++i) {
		index = nodeIndexes_[i];
		nodes_[index]->minDist = std::numeric_limits<FPType>::infinity( );
		nodes_[index]->maxDist = 0.0;
	}
	nodes_[originIndex_]->minDist = 0.0; // set to zero for origin 
	
	// find shortest and longest paths with positive flow
	FPType dist = 0.0;
	FPType time = 0.0;
	int i = beginAscPass();
	
	assert(originIndex_ == i); //-- TODO: theoretically there might be 
					// an alternative top order that starts with another node -- 
					// not clear what to do in this case
	StarLink* link = NULL;
	for (; i != -1; i = getNextAscPass()) {
		std::list<StarLink*> &linksList = nodes_[i]->incomeLinks;
		for (std::list<StarLink*>::iterator it = linksList.begin(); it != linksList.end(); ++it) {
			link = *it;
			index = link->getNodeFromIndex();
			assert(nodes_[index] != NULL);
			time = link->getTime();
			dist = nodes_[index]->minDist + time; // min dist
			if (dist < nodes_[i]->minDist) {
				nodes_[i]->minDist = dist;
				nodes_[i]->minLink = link; 
			}
			dist = nodes_[index]->maxDist + time; // max dist
			if (dist >= nodes_[i]->maxDist) {
				nodes_[i]->maxDist = dist;
				nodes_[i]->maxLink = link; 
			}
		}
		
		if (i == destIndex) break;
	}
};

bool DAGraph::addBetterLinks(){
	bool wasImproved = false;
	bool newLinkAdded = false;
	p2Cont_.clear();
	for (int linkIndex = getNextNotInSet(-1); linkIndex != -1;
			linkIndex = getNextNotInSet(linkIndex)) {
		StarLink *link = net_->getLink(linkIndex);
		if (isReachable(link) && worthAdding(link)) {
			if (addLink(link)) newLinkAdded = true;
			wasImproved = true;
		}
	}
	if (!wasImproved) {
		newLinkAdded = addFromP2();
	}

	return newLinkAdded; 
};

bool DAGraph::addFromP2(){
	bool addedFromP2 = false;
	if (!p2Cont_.empty()) {
		for (std::list<int>::iterator it = p2Cont_.begin(); it != p2Cont_.end(); ++it) {
			if (addLink(net_->getLink(*it))) addedFromP2 = true;
		}
	}
	return addedFromP2;
};

bool DAGraph::worthAdding(StarLink* link){ 
	assert(link);
	FPType linkTime = link->getTime();
	int nodeFromIndex = link->getNodeFromIndex();
	int nodeToIndex = link->getNodeToIndex();
	assert(nodes_[nodeFromIndex] && nodes_[nodeToIndex]);
	if ((nodes_[nodeFromIndex])->maxDist + linkTime < (nodes_[nodeToIndex])->maxDist) { // check p2
		p2Cont_.push_back(link->getIndex());
		if (linkTime + (nodes_[nodeFromIndex])->minDist < (nodes_[nodeToIndex])->minDist) { // check p1
			return true;
		}
	}
	return false;
};

bool DAGraph::isReachable(StarLink* link) const {
	assert(link);
	return (nodes_[link->getNodeFromIndex()] != NULL && nodes_[link->getNodeToIndex()] != NULL);
};

// Always check if both nodes are reachable from a given origin after returning 
// the link that is not yet in the set!!!!
int DAGraph::getNextNotInSet(int startIndex) const{
	int nbLinks = net_->getNbLinks();
	assert(startIndex >= -1 && startIndex < nbLinks);
	if (startIndex == nbLinks - 1) return -1;
	int index = startIndex + 1;
	while (links_[index] != NULL) {
		++index;
		if (index == nbLinks) return -1;
	}
	return index;
};

bool DAGraph::addLink(StarLink *link){
	assert(link !=  NULL);
	int index = link->getIndex();
	bool wasAdded = false;
	if (links_[index] == NULL) { // check if this link has already been added
		
		int nodeFromIndex = link->getNodeFromIndex();
		int nodeToIndex = link->getNodeToIndex();
		StarNode *nodeFrom = net_->getNodeWithLinks(nodeFromIndex);
		StarNode *nodeTo = net_->getNodeWithLinks(nodeToIndex);
		FPType demandTo = 0.0;
		if (nodes_[nodeToIndex] == NULL) {
			demandTo = mat_->getDemandByIndex(originIndex_, nodeToIndex);
		} else {
			demandTo = getDemand(nodeToIndex);
		}
		
		if ((nodeTo == NULL || !nodeTo->getIsZone() || demandTo > 0.0) && 
				(nodeFrom == NULL || !nodeFrom->getIsZone() 
					|| nodeFrom->getIndex() == originIndex_)) {
		
			links_[index] = link;
			wasAdded = true;
			++linkSize_;
			linkIndexes_.push_back(index);
			
			if (nodes_[nodeFromIndex] == NULL) {
				FPType demandFrom = mat_->getDemandByIndex(originIndex_, nodeFromIndex);
				DAGraphNode *newNode = new DAGraphNode(nodeFromIndex, demandFrom); 
				nodes_[nodeFromIndex] = newNode;
				nodeIndexes_[nodeSize_] = nodeFromIndex;
				++nodeSize_;
			}
			if (nodes_[nodeToIndex] == NULL) {
				DAGraphNode *newNode = new DAGraphNode(nodeToIndex, demandTo); 
				nodes_[nodeToIndex] = newNode;
				nodeIndexes_[nodeSize_] = nodeToIndex;
				++nodeSize_;
			} 
			(nodes_[nodeToIndex])->incomeLinks.push_back(link);
			(nodes_[nodeFromIndex])->outLinks.push_back(link);
		}
	}
	return wasAdded;
};

bool DAGraph::removeLink(int index){
	
	if (links_[index] != NULL) {
		if (fabs(originFlows_[index]) <= zeroFlow_){
			int nodeToIndex = (links_[index])->getNodeToIndex();
			if ((nodes_[nodeToIndex])->incomeLinks.size() > 1) {
				(nodes_[nodeToIndex])->incomeLinks.remove(links_[index]);
				(nodes_[(links_[index])->getNodeFromIndex()])->outLinks.remove(links_[index]);
				links_[index] = NULL;
				--linkSize_;
				return true;
			}
		}
	}
	return false;
};

bool DAGraph::removeUnusedLinks(const std::list<StarLink*> &links){
	int index = -1;
	bool wasDeleted = false;
	for(std::list<StarLink*>::const_iterator it = links.begin(); it != links.end(); ++it){
		index = (*it)->getIndex();
		if (removeLink(index)) {
			wasDeleted = true;
			linkIndexes_.remove(index);
		}
	}
	return wasDeleted;
	
};

// at the moment No topological order is maintained!
bool DAGraph::removeUnusedLinks(){
	bool wasDeleted = false;
	for(std::list<int>::iterator it = linkIndexes_.begin(); it != linkIndexes_.end(); ++it){
		if (removeLink(*it)) {
			it = linkIndexes_.erase(it);
			wasDeleted = true;
			--it;
		}
	}
	return wasDeleted;
};

std::list<StarLink*> DAGraph::getOutLinksCopy(int nodeIndex) const{
	return (nodes_[nodeIndex])->outLinks;
};

std::list<StarLink*> DAGraph::getInLinksCopy(int nodeIndex) const{
	return (nodes_[nodeIndex])->incomeLinks;
};

void DAGraph::getOutLinks(int nodeIndex, std::list<StarLink*>& listRef) {
	listRef = (nodes_[nodeIndex])->outLinks;
};

void DAGraph::getInLinks(int nodeIndex, std::list<StarLink*>& listRef) {
	listRef = (nodes_[nodeIndex])->incomeLinks;
};


int DAGraph::beginAscPass(){
	if (nodeSize_ == 0) return -1;
	currNode_ = topOrder_.begin();
	if (currNode_ == topOrder_.end()) return -1;
	return *currNode_;
};

int DAGraph::getNextAscPass(){
	++currNode_;
	if (currNode_ == topOrder_.end()) return -1;
	return *currNode_;
};

int DAGraph::beginDescPass(){
	if (nodeSize_ == 0) return -1;
	currNodeDesc_ = topOrder_.rbegin();
	if (currNodeDesc_ == topOrder_.rend()) return -1;
	return *currNodeDesc_;
};

int DAGraph::getNextDescPass(){
	++currNodeDesc_;
	if (currNodeDesc_ == topOrder_.rend()) return -1;
	return *currNodeDesc_;
};

void DAGraph::print() const{
	int index = -1;
	std::cout << "ORIGIN = " << getOriginIndex() << std::endl;
	for (int i = 0; i < nodeSize_; ++i) {
		index = nodeIndexes_[i];
		std::cout << "Node " << (nodes_[index])->nodeIndex; 
		std::cout << " in-links: ";
		std::list<StarLink*> &links2 = (nodes_[index])->incomeLinks;
		for(std::list<StarLink*>::iterator it = links2.begin(); it != links2.end(); ++it){
			std::cout << "link " << (*it)->getIndex() << " (" << (*it)->getNodeFromIndex() << 
				", " << (*it)->getNodeToIndex() << ") OriginFlow = " << 
				originFlows_[(*it)->getIndex()] << " ";
		}
		std::cout << std::endl;
		
	}
	
};

void DAGraph::printOriginFlow() const{
	std::cout << "Origin = " << originIndex_ << " nbLinks = " << net_->getNbLinks() << std::endl;
	for (int i = 0; i < net_->getNbLinks(); ++i) {
		std::cout << originFlows_[i] << " ";
	}
	std::cout << std::endl;
};

void DAGraph::setOriginFlow(int index, FPType flow){
	assert(index >= 0 && index < net_->getNbLinks());
	originFlows_[index] = flow;
};

FPType DAGraph::getOriginFlow(int linkIndex) const{
	//assert(linkIndex >= 0 && linkIndex < net_->getNbLinks());
	return originFlows_[linkIndex];
};

void DAGraph::printShPath(int node){
	StarLink *prevMin = (nodes_[node])->minLink;
	int prevNode = -1; 
	std::cout << "Cost = " << (nodes_[node])->minDist << std::endl;
	FPType cost = 0.0;
	while (prevMin != NULL){
		prevNode = prevMin->getNodeFromIndex();
		cost += prevMin->getTime();
		std::cout << "[" << prevMin->getNodeToIndex() << ", " << prevNode << "] " 
				<< prevMin->getFlow() << "-";
		prevMin = (nodes_[prevNode])->minLink;
	}
	std::cout << std::endl;
};

void DAGraph::printMaxShPath(int node){
	StarLink *prevMax = (nodes_[node])->maxLink;
	int prevNode = -1; 
	std::cout << "Cost = " << (nodes_[node])->maxDist << std::endl;
	FPType cost = 0.0;
	while (prevMax != NULL){
		prevNode = prevMax->getNodeFromIndex();
		cost += prevMax->getFlow() * prevMax->getTime();
		std::cout << "[" << prevMax->getNodeToIndex() << ", " << prevNode << "] " 
					<< prevMax->getFlow() << " - ";
		prevMax = (nodes_[prevNode])->maxLink;
	}
	std::cout << std::endl;
};	

DAGraphNode* DAGraph::getNode(int index) const{
	assert(nodes_[index] != NULL);
	return nodes_[index];
};

FPType DAGraph::checkOFlowsFeasibility(){
	int nbNodes = net_->getNbNodes(); 
	FPType total[nbNodes];
	for (int i = 0; i < nbNodes; ++i) {
		total[i] = 0.0;
	}
	// load demands
	for (OriginIterator it = mat_->begin(); it != mat_->end(); ++it){
		Origin* origin = *it;
		if (origin->getIndex() == originIndex_){
			for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
				PairOD* dest = *jt;
				FPType demand = dest->getDemand();
				total[origin->getIndex()] += demand;
				total[dest->getIndex()] -= demand;
			}
			break;
		}
	}
	//travers network and check
	
	int i = -1;
	StarLink* link = NULL;
	std::list<StarLink*> inLinks;
	for (int j = 0; j < nodeSize_; ++j) {
    	i = nodeIndexes_[j];
    	getInLinks(i, inLinks);
    	for (std::list<StarLink*>::iterator it = inLinks.begin(); it != inLinks.end(); ++it){
      		link = *it;
			total[link->getNodeFromIndex()] -= getOriginFlow(link->getIndex());
			total[link->getNodeToIndex()] += getOriginFlow(link->getIndex());
		}
	
	}
	
	FPType max = 0.0;
		
	for (int i = 0; i < net_->getNbNodes(); ++i) {
		if (fabs(total[i]) > max) {	
			max = fabs(total[i]);
		}
	}
	return max;
};

FPType DAGraph::getNbLinksInBush() const{
	return linkSize_; 
};
