#include "Path.h"
#include "StarLink.h"

#include <iostream>

Path::Path() : flow_(0.0), prevFlow_(0.0), currCost_(0.0){ 

};

Path::~Path(){

};

void Path::setPrevFlow(FPType flow){
	prevFlow_ = flow;
};

void Path::setFlow(FPType flow){
	flow_ = flow;
};

void Path::setCurrCost(FPType cost){
	currCost_ = cost;
};

void Path::addLinkToPath(StarLink *link){
	links_.push_front(link);
};

void Path::pushBackLink(StarLink* link) {
	links_.push_back(link);
};
		
FPType Path::getFlow() const{
	return flow_;
};

FPType Path::getCurrCost() const{
	return currCost_;
};

StarLinkIterator Path::begin() {
	return links_.begin();
};

StarLinkIterator Path::end() {
	return links_.end();
};

ConstStarLinkIterator Path::begin() const{
	return links_.begin();
};

ConstStarLinkIterator Path::end() const {
	return links_.end();
};

void Path::print(){
	std::cout << " flow = " << flow_  << " cost = " << currCost_ << std::endl;
	for (StarLinkIterator it = begin(); it != end(); ++it) {
		StarLink* link = *it;
		StarLink* forwardLink = link->getForwardLink();
		std::cout << forwardLink->getIndex() << " [" << forwardLink->getTime() << ", " << forwardLink->getFlow() <<
		 "] ";
	}
	std::cout << std::endl;
};

FPType Path::getPrevFlow() const{
	return prevFlow_;
};

bool Path::isEmpty() const{
	return  links_.empty();
};

int Path::size() const{
	return links_.size();
};

void Path::clear() {
	links_.clear();
};

FPType Path::calcCurrTime() const {
	FPType pathTime = 0.0;
	for (ConstStarLinkIterator it = begin(); it != end(); ++it) {
		pathTime += (*it)->getTime();
	}
	return pathTime;
};