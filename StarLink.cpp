#include "StarLink.h"

#include <sstream>

StarLink::StarLink(int nodeFrom, int nodeTo, LinkFnc *fnc) : index_(-1),  
				  nodeFrom_(nodeFrom), nodeTo_(nodeTo),
				  nodeFromIndex_(-1), nodeToIndex_(-1), flow_(0.0), fnc_(fnc), der_(0.0) { 
	updateTime();
};

StarLink::~StarLink(){
	delete fnc_;
	fnc_ = NULL;
};

int StarLink::getNodeFrom() const{
	return nodeFrom_;
};

int StarLink::getNodeTo() const{
	return nodeTo_;
};

LinkFnc* StarLink::getLinkFnc() const{
	return fnc_;
};

FPType StarLink::getFlow() const{
	return flow_;
};

void StarLink::setFlow(FPType flow){
	flow_ = flow;
};

int StarLink::getIndex() const{
	return index_;
};

void StarLink::setIndex(int index){
	index_ = index;
};

int StarLink::getNodeFromIndex() const{
	return nodeFromIndex_;
};

int StarLink::getNodeToIndex() const{
	return nodeToIndex_;
};

FPType StarLink::getTime() const{
	return time_;
};
		
void StarLink::setNodeToIndex(int index){
	nodeToIndex_ = index;
};

void StarLink::setNodeFromIndex(int index){
	nodeFromIndex_ = index;
};

void StarLink::addFlow(FPType flow){
	flow_ += flow;
};

void StarLink::updateTime(){
	time_ = fnc_->evaluateTimeAndDerivative(flow_, der_); 
};

void StarLink::setFlowToZero(){
	flow_ = 0.0;
};

std::string StarLink::toString() const{
	std::stringstream ss;
	ss << "Link: " << index_ << " [" << nodeFromIndex_ << ", " << nodeToIndex_ << "]" 
		<<	" [" << nodeFrom_ << ", " << nodeTo_ << "]";
	return ss.str();
};

FPType StarLink::getDerivative() const{
	return der_;
};

StarLink* StarLink::getForwardLink() {
	return this;
};
