#include "LinearIndiffCurve.h"

#include <sstream>
#include <cassert>

LinearIndiffCurve::LinearIndiffCurve(FPType time1, FPType time2, TollType toll2) :
	a_((time2 - time1) / toll2), b_(time1) {
	assert(toll2 > 0 && time1 >= 0 && time2 >= 0);
};

LinearIndiffCurve::LinearIndiffCurve(FPType a, FPType b) : a_(a), b_(b) {

};

LinearIndiffCurve::~LinearIndiffCurve() {

};

FPType LinearIndiffCurve::evaluate(TollType toll) {
	return a_ * toll + b_;
};

std::string LinearIndiffCurve::toString() {
	std::stringstream ss;
	ss << "Linear: (" << a_ << ", " << b_ << ")";
	return ss.str();
};