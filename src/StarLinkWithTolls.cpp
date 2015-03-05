#include "StarLinkWithTolls.h"

StarLinkWithTolls::StarLinkWithTolls(StarLink* link, TollType toll) : 
	StarLinkWithReverseLink(link), toll_(toll) {
};

StarLinkWithTolls::~StarLinkWithTolls() {

};

FPType StarLinkWithTolls::getTime() const {
	return toll_;
};
