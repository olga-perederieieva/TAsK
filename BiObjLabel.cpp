#include "BiObjLabel.h"
#include "StarLink.h"

#include <stdlib.h>
#include <sstream>
#include <cassert>

#include <iomanip>  

BiObjLabel::BiObjLabel(FPType time, TollType toll, int owner, BiObjLabel* prevLabel,
						StarLink* prevLink) : 
						time_(time), toll_(toll), owner_(owner),
						prevLabel_(prevLabel), prevLink_(prevLink) {

};

BiObjLabel::~BiObjLabel(){

};

FPType BiObjLabel::getTime() const {
	return time_;
};

TollType BiObjLabel::getToll() const {
	return toll_;
};

BiObjLabel* BiObjLabel::getPrevLabel() const {
	return prevLabel_;
};

int BiObjLabel::getOwner() const {
	return owner_;
};

StarLink* BiObjLabel::getPrevLink() const {
	return prevLink_;
};


std::string BiObjLabel::toString() const{
	std::stringstream ss;
	ss << std::setprecision(20) << "time: " << getTime() << " toll: " << getToll() << ";" <<
		" owner: " << owner_ ;
		if (getPrevLink() != NULL) ss << " preLink: " << getPrevLink()->toString();
		 if (getPrevLabel() != NULL) {
		 	ss << " prevLabel: owner = " << getPrevLabel()->getOwner() << 
		 	"[" << getPrevLabel()->getTime() << ", " << getPrevLabel()->getToll() << "] link: ";
		 }
	return ss.str();
};

void BiObjLabel::setTime(FPType timeVal) {
	time_ = timeVal;
};