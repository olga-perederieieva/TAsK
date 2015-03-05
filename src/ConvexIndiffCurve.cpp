#include "ConvexIndiffCurve.h"

#include <sstream>
#include <math.h>
#include <cassert>

ConvexIndiffCurve::ConvexIndiffCurve(FPType time1, FPType time2, TollType toll2) :
	a_((time2 - time1)/pow(toll2, 3)), b_(time1) {
	assert(toll2 > 0 && time1 >= 0 && time2 >= 0);
};

ConvexIndiffCurve::ConvexIndiffCurve(FPType a, FPType b) : a_(a), b_(b) {

};

ConvexIndiffCurve::~ConvexIndiffCurve(){

};

FPType ConvexIndiffCurve::evaluate(TollType toll) {
	return a_ * pow(toll, 3) + b_;
};

std::string ConvexIndiffCurve::toString() {
	std::stringstream ss;
	ss << "Convex: (" << a_ << ", " << b_ << ")"; 
	return ss.str();
};