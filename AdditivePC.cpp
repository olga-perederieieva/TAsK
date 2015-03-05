#include "AdditivePC.h"
#include "StarLink.h"
#include "Path.h"

AdditivePC::AdditivePC(){

};

AdditivePC::~AdditivePC(){

};

FPType AdditivePC::calculate(Path *path, int odIndex) const{
	FPType cost = 0.0;
	for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
		cost += (*it)->getTime();
	}
	return cost;
};
