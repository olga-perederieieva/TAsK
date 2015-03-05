#include "StarNetworkWithTolls.h"
#include "StarLinkTolls.h"
#include "StarNode.h"
#include "StarNetwork.h"

#include <cassert>

StarNetworkWithTolls::StarNetworkWithTolls(const TollContainerType& tolls) : tolls_(tolls){

};

StarNetworkWithTolls::~StarNetworkWithTolls(){

};

StarNetwork* StarNetworkWithTolls::createNetWithTolls(const StarNetwork & net){
	
	std::string net_Name = net.getNetName();
	std::string & netName = net_Name;
	
	StarNetwork* netWithTolls = new StarNetwork(net.getNbNodes(), net.getNbLinks(),  netName);	
	
	for (StarNode* node = net.beginNode(); node != NULL; node = net.getNextNode()) {
		netWithTolls->addNode(new StarNode(node->getID(), node->getIsZone()));		
		for(StarLink* link = net.beginLink();link != NULL; link = net.getNextLink()) {
			StarLink* newLink = createLink(link);
			netWithTolls->addLink(newLink);
		}
	}

	netWithTolls->linkNodes();
	
	for (StarNode* node = netWithTolls->beginNode(); node != NULL; node = netWithTolls->getNextNode()) {	
		for(StarLink* link = netWithTolls->beginLink();link != NULL; link = netWithTolls->getNextLink()) {
			assert(link->getIndex() == link->getForwardLink()->getIndex());
		}
	}

	return netWithTolls;
};


StarLink* StarNetworkWithTolls::createLink(StarLink* link) {
	return new StarLinkTolls(link, tolls_[link->getIndex()]);
};
