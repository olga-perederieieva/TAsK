#include "PathAndDirection.h"

#include <stdlib.h>

PathAndDirection::PathAndDirection() : path_(NULL), directionVal_(0.0) {

};

PathAndDirection::~PathAndDirection(){

};

Path* PathAndDirection::getPath() const{
	return path_;
};

FPType PathAndDirection::getDirectionValue() const{
	return directionVal_;
};
		
void PathAndDirection::setPath(Path *path){
	path_ = path;
};

void PathAndDirection::setDirection(FPType direction){
	directionVal_ = direction;
};
