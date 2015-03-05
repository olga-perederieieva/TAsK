#include "ReverseNetworkWithTolls.h"
#include "StarLinkWithTolls.h"

ReverseNetworkWithTolls::ReverseNetworkWithTolls(const TollContainerType& tolls) : tolls_(tolls){

};

ReverseNetworkWithTolls::~ReverseNetworkWithTolls(){

};

StarLink* ReverseNetworkWithTolls::createLink(StarLink* link) {
	return new StarLinkWithTolls(link, tolls_[link->getIndex()]);
};