#include "ReverseNetwork.h"
#include "StarLinkWithReverseLink.h"
#include "StarNetwork.h"
#include "StarNode.h"

#include <vector>
#include <list>
#include <cassert>
#include <iostream>

ReverseNetwork::ReverseNetwork(){};

ReverseNetwork::~ReverseNetwork(){};

StarNetwork* ReverseNetwork::createReverseStar(const StarNetwork & net){

	std::string net_Name = net.getNetName();
	std::string & netName = net_Name;

	StarNetwork* reversed = new StarNetwork(net.getNbNodes(), net.getNbLinks(),  netName);

	std::vector<int> nodeID(net.getNbNodes(), 0);
	std::vector<bool> nodeIsZone(net.getNbNodes(), false);
	std::vector<std::list<StarLink*> > links(net.getNbNodes());

	 for(StarNode* node = net.beginNode();node != NULL; node = net.getNextNode()){
	 	
	 	nodeIsZone[node->getIndex()] = node->getIsZone();
		for(StarLink* link = net.beginLink();link != NULL; link = net.getNextLink()){
			links[link->getNodeToIndex()].push_back(link);
			nodeID[link->getNodeToIndex()] = link->getNodeTo();
		}
	}

	for(int i = 0; i < net.getNbNodes(); ++i){
		if(!links[i].empty()){
			assert(nodeID[i] != 0);
			reversed->addNode(new StarNode(nodeID[i], nodeIsZone[i]) );
			for (std::list<StarLink*>::iterator it = links[i].begin(); it != links[i].end(); ++it){
				StarLink* currentLink = *it;
				StarLink* newLink = createLink(currentLink);
				reversed->addLink(newLink);
			}
		}
	}

	reversed->linkNodes();

	// nodes are added in the same order as in original network ->
	// hence, all node indexes are the same
	assert(checkNodeIndexes(net, reversed));

	return reversed;
};

StarLink* ReverseNetwork::createLink(StarLink* link) {
	return new StarLinkWithReverseLink(link);
};

bool ReverseNetwork::checkNodeIndexes(const StarNetwork & net, StarNetwork* reversedNet) {
	int nbNodes = net.getNbNodes();
	for (int forwardNodeIndex = 0; forwardNodeIndex < nbNodes; ++forwardNodeIndex) {
		int nodeID = net.findID(forwardNodeIndex);
		if (nodeID != -1) {
			int reversedIndex = reversedNet->getNodeIndex(nodeID);
			if (forwardNodeIndex != reversedIndex) {
				std::cout << "AHA--------------- forwardNodeIndex = " << forwardNodeIndex <<
						" nodeID = " << nodeID << " reversedIndex = " << reversedIndex << std::endl;
				return false;
			}
		}
	}
	return true;
};