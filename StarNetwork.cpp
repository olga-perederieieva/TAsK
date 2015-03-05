#include "StarNetwork.h"
#include "Error.h"
#include "FileReader.h"
#include "StarLink.h"
#include "StarNode.h"
#include "FileWriter.h"

#include <cassert>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <algorithm>  

StarNetwork::StarNetwork(int nbNodes, int nbLinks, std::string &netName) : 
							netName_(netName), nbNodes_(nbNodes), nbLinks_(nbLinks),
							nodes_(nbNodes), links_(nbLinks), pointers_(nbNodes + 1), 
							size_(0), sizeLinks_(0), curNode_(-1), curLink_(-1), curOnlyLink_(-1),
							 linkAdded_(true){
	for (int i = 0; i < nbNodes; ++i) { 
		nodes_[i] = NULL;
		pointers_[i] = -1;
	}
	pointers_[nbNodes] = -1;
	
	for (int i = 0; i < nbLinks; ++i) {
		links_[i] = NULL;
		
	}
	std::cout << "link0 = " << links_[0] << std::endl;

};

StarNetwork::~StarNetwork(){
	for (int i = 0; i < size_; ++i) delete nodes_[i];
	for (int i = 0; i < nbLinks_; ++i) delete links_[i];

};

void StarNetwork::linkNodes(){
	assert(size_ > 0);
	if (pointers_[size_ - 1] == sizeLinks_) 
		throw Error("Last added node does not have out-going links");
	createIndexes();
	bool nodeToFound = false;
	for (StarLink *link = beginOnlyLink(); link != NULL; link = getNextOnlyLink())  {
		nodeToFound = false;
		for (StarNode *node = beginNode(); node != NULL; node = getNextNode()) {
			if (link->getNodeTo() == node->getID()){
				link->setNodeToIndex(node->getIndex());
				nodeToFound = true;
				break;
			}	
		}
		if (!nodeToFound){
			link->setNodeToIndex(getNodeIndex(link->getNodeTo()));
		}
	}
};
void StarNetwork::createIndexes(){
	std::tr1::unordered_map<int, int>::const_iterator got;
	int id = -1;
	int count = size_;
	for (StarLink *link = beginOnlyLink(); link != NULL; link = getNextOnlyLink()){
		id = link->getNodeTo(); // only nodeTo is considered because all nodes with 
								// out-going links were added 
								// during construction
		got = idMap_.find(id);
		if (got == idMap_.end()) {
			idMap_.insert(std::make_pair<int, int>(id, count));
			++count;
			assert(count <= nbLinks_);
		}
	}
};

void StarNetwork::addNode(StarNode *node){
	if (size_ >= nbNodes_) throw 
			Error("Attemp to add a node when the number of nodes exceeds the total number of nodes");
	if (!linkAdded_)
		throw Error("Two nodes were added in a row. Only nodes with out-going links can be added");
	node->setIndex(size_);
	nodes_[size_] = node;
	idMap_.insert(std::make_pair<int, int>(node->getID(), size_)); 
	pointers_[size_] = sizeLinks_;
	++size_;
	pointers_[size_] = nbLinks_;
	linkAdded_ = false;
};

void StarNetwork::addLink(StarLink *link){
	if (sizeLinks_ >= nbLinks_) throw 
		Error("Attemp to add a link when the number of links exceeds the total number of links");
	if (size_ == 0) throw Error("Node must be added first");
	links_[sizeLinks_] = link;
	link->setIndex(sizeLinks_);
	link->setNodeFromIndex(size_ - 1);
	++sizeLinks_;
	linkAdded_ = true;
};
		
std::string StarNetwork::getNetName() const {
	return netName_;
};

int StarNetwork::getNbNodes() const {
	return nbNodes_;
};

int StarNetwork::getNbLinks() const {
	return nbLinks_;
};

StarNode* StarNetwork::beginNode() const {
	curNode_ = 0;
	curLink_ = pointers_[curNode_];
	return nodes_[curNode_];
};

StarLink* StarNetwork::getLink(int linkIndex) const{
	assert((linkIndex >= 0) && (linkIndex < nbLinks_));
	return links_[linkIndex];
};

StarNode* StarNetwork::beginNode(int index) const {
	assert((index >= 0) && (index < nbNodes_));
	if (index >= size_) return NULL;
	curNode_ = index; 
	curLink_ = pointers_[curNode_];
	return nodes_[curNode_];
};

StarNode* StarNetwork::getNextNode() const{
	++curNode_;
	if (curNode_ == size_) {
		curLink_ = -1;
		return NULL;
	}
	curLink_ = pointers_[curNode_];
	return nodes_[curNode_];
};

StarLink* StarNetwork::beginLink() const{
	return links_[curLink_];
};

StarLink* StarNetwork::getNextLink() const{
	++curLink_;
	if (curLink_ == pointers_[curNode_ + 1]) {
		return NULL;
	}
	return links_[curLink_];	
};

StarLink* StarNetwork::beginOnlyLink() const {
	curOnlyLink_ = 0;
	return links_[0];
};

StarLink* StarNetwork::getNextOnlyLink() const {
	++curOnlyLink_;
	if (curOnlyLink_ == nbLinks_) return NULL;
	return links_[curOnlyLink_];
};
		
void StarNetwork::print(){
	std::cout << "network name: " << getNetName() << " nbNodes = " << nbNodes_ 
			  << " nbLinks = " << nbLinks_	 << std::endl;
	for (StarNode *node = beginNode(); node != NULL; node = getNextNode()){
	  	std::cout << "Node ID = " << node->getID()  << " node index = " << node->getIndex() 
	  			  << std::endl;
		for (StarLink *link = beginLink(); link != NULL; link = getNextLink()) {
		  std::cout << " link " << link->getIndex() << "  (" << link->getNodeFrom() << ", " << 
		  	link->getNodeTo() << ") cost-> (" << link->getTime() << ") flow = " 
			<< link->getFlow() << std::endl;
		}
	}
};

int StarNetwork::getNodeIndex(int id){
	std::tr1::unordered_map<int, int>::const_iterator got = idMap_.find(id);
	if (got == idMap_.end()) {
		std::stringstream str;
		str << "Cannot find id = " << id;
		throw Error(str.str());
	}
	return got->second;
};

void StarNetwork::printToFile(const std::string &fileName, int precision){
	
	FileWriter fw(fileName);
	std::string line;
	std::ostringstream strs;
					
	for (StarLink *link = beginOnlyLink(); link != NULL; link = getNextOnlyLink()) {
		strs.str("");
		strs << link->getNodeFrom() << " " << link->getNodeTo() << " " << 
			 std::setprecision(precision) << 
		link->getFlow() << " " << link->getTime() << "\n";
		line = strs.str();
		fw.writeLine(line);
	}	
	
};

bool StarNetwork::assignLinkFlow(int nodeFrom, int nodeTo, FPType flow){
	for (StarLink *link = beginOnlyLink(); link != NULL; link = getNextOnlyLink()) {
		if ((link->getNodeFrom() == nodeFrom) && (link->getNodeTo() == nodeTo)){
			link->setFlow(flow);
			return true;
		}
	}
	return false;
};

void StarNetwork::loadFromFile(const std::string& fileName){
	FileReader myfile(fileName);
	int nodeFrom = -1;
	int nodeTo = 1;
	FPType flow = 0.0;
	FPType tmp = 0.0;
	while ( myfile.isGood() ){
      	std::string line = myfile.getNextLine();
      	std::stringstream ss(line);
      	ss >> nodeFrom >> nodeTo >> flow >> tmp;
      	if (!assignLinkFlow(nodeFrom, nodeTo, flow))
      		throw Error("This link does not exist in the network");
    }
};

void StarNetwork::calculateLinkCosts(){
	for (StarLink *link = beginOnlyLink(); link != NULL; link = getNextOnlyLink()) {
		link->updateTime();
	}
};

StarNode* StarNetwork::getNodeWithLinks(int index){
	assert(index >= 0);
	if (index >= size_) return NULL; // it means that this node does not have out-going links
	return nodes_[index];
};


int StarNetwork::findID(int index) const {
	for(std::tr1::unordered_map<int, int>::const_iterator it = idMap_.begin();
			it != idMap_.end(); ++it){
		if (it->second == index) {
			return it->first;
		}
	}
	return -1;
};
