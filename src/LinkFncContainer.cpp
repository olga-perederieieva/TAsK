#include "LinkFncContainer.h"
#include "StarNetwork.h"
#include "StarLink.h"

LinkFncContainer::LinkFncContainer(StarNetwork *net) : links_(net->getNbLinks())
					{
	
	int nbLinks = net->getNbLinks();
	for (int i = 0; i < nbLinks; ++i) {
		links_[i] = NULL;
	}
	for (StarLink *link = net->beginOnlyLink(); link != NULL; link = net->getNextOnlyLink()){
		links_[link->getIndex()] = link->getLinkFnc();
	}
};

LinkFncContainer::~LinkFncContainer(){
};

LinkIterator LinkFncContainer::begin(){
	return links_.begin();
};

LinkIterator LinkFncContainer::end(){
	return links_.end();
};

LinkFnc* LinkFncContainer::getLinkFnc(int index){
	return links_[index];
};
