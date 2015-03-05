#include "StarLinkTolls.h"

StarLinkTolls::StarLinkTolls(StarLink* link, TollType toll) : StarLink(link->getNodeFrom(), 
						link->getNodeTo(), link->getLinkFnc()->clone()), toll_(toll),
						originalLink_(link){
};

StarLinkTolls::~StarLinkTolls() {

};

FPType StarLinkTolls::getTime() const {
	return toll_;
};

StarLink* StarLinkTolls::getForwardLink() {
	return originalLink_;
};
