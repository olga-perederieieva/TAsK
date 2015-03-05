#include "ConcaveIndiffCurve.h"

#include <sstream>
#include <math.h>
#include <cassert>

ConcaveIndiffCurve::ConcaveIndiffCurve(FPType time1, FPType time2, TollType toll2) :
	a_((time2 - time1) / pow(toll2, 1.0/3)), b_(time1) {
	assert(toll2 > 0 && time1 >= 0 && time2 >= 0);
};

ConcaveIndiffCurve::ConcaveIndiffCurve(FPType a, FPType b) : a_(a), b_(b) {

};

ConcaveIndiffCurve::~ConcaveIndiffCurve() {

};

FPType ConcaveIndiffCurve::evaluate(TollType toll) {
	return a_ * pow(toll, 1.0/3) + b_;
};

std::string ConcaveIndiffCurve::toString() {
	std::stringstream ss;
	ss << "Concave: (" << a_ << ", " << b_ << ")"; 
	return ss.str();
};