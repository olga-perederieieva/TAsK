#include "StarNode.h"

StarNode::StarNode(int id, bool isZone) : id_(id), index_(-1), isZone_(isZone){

};

StarNode::~StarNode(){

};

int StarNode::getID(){
	return id_;
};

bool StarNode::getIsZone(){
	return isZone_;
};

void StarNode::setIndex(int index){
	index_ = index;
};

int StarNode::getIndex(){
	return index_;
};
