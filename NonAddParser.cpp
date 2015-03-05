#include "NonAddParser.h"

#include <cassert>

NonAddParser::NonAddParser(const std::string& fileWithNetwork) : 
		DefaultNetParser(fileWithNetwork), tolls_(NULL) {

};

NonAddParser::~NonAddParser() {

};

void NonAddParser::allocateTolls(int nbLinks){
	assert(nbLinks > 0);
	tolls_ =  new TollContainerType(nbLinks);
};

void NonAddParser::setToll(int linkIndex, TollType toll){
	(*tolls_)[linkIndex] = toll;
};	

TollContainerType* NonAddParser::getTolls(){
	return tolls_;
};