#include "StarLinkWithReverseLink.h"

#include <cassert>

StarLinkWithReverseLink::StarLinkWithReverseLink(StarLink* link) : StarLink(link->getNodeTo(), 
						link->getNodeFrom(), link->getLinkFnc()->clone()), originalLink_(link) {
	assert(link != NULL);
};

StarLinkWithReverseLink::~StarLinkWithReverseLink() {

};

FPType StarLinkWithReverseLink::getTime() const {
	return originalLink_->getTime();
};

StarLink* StarLinkWithReverseLink::getForwardLink() {
	return originalLink_;
};