#include "PieceWiseLinearIndiffCurve.h"
#include "Error.h"

#include <cassert>
#include <sstream>
#include <algorithm>

PieceWiseLinearIndiffCurve::PieceWiseLinearIndiffCurve(int nbPoints) : nbPoints_(nbPoints), 
		size_(0), values_(nbPoints, 0), tolls_(nbPoints, 0), valuesCopy_(nbPoints, 0),
		tollsCopy_(nbPoints, 0) {
	assert(nbPoints > 0);
}; 

PieceWiseLinearIndiffCurve::~PieceWiseLinearIndiffCurve() {
}; 

int PieceWiseLinearIndiffCurve::getNbPoints() const{
	return size_;
};

void PieceWiseLinearIndiffCurve::addPoint(TollType toll, FPType value) {
	assert(size_ < nbPoints_);
	if (toll < 0 || value < 0) throw Error("All values of indifference curve must be non-negative");
	if (size_ == 0 && toll != 0) throw Error("First value of toll must be zero");
	if (size_ > 0 && (toll <= tolls_[size_ - 1] || value < values_[size_ - 1])) 
		throw Error ("Attempt to add toll that is less or equal to previous toll");
	tolls_[size_] = toll;
	values_[size_] = value;
	tollsCopy_[size_] = toll;
	valuesCopy_[size_] = value;
	++size_;
}; 

FPType PieceWiseLinearIndiffCurve::evaluate(TollType toll) {
	assert(toll >= 0);
	if (toll == 0) return values_[0];
	int i = 0;
	while (toll - 1e-10 > tolls_[i]) {
		++i;
		// toll is bigger than the last added point
		if (i == nbPoints_) {
			return toll - tolls_[i - 1] + values_[i - 1];
		}
	}
	return values_[i - 1] + (toll - tolls_[i - 1]) * (values_[i] - values_[i - 1]) / 
				(tolls_[i] - tolls_[i - 1]);
}; 

std::string PieceWiseLinearIndiffCurve::toString() {
	std::stringstream retVal("");
	for (int i = 0; i < size_; ++i) {
		retVal << "(" << tolls_[i] << ", " << values_[i] << ") ";
	}
	return retVal.str();
}; 

template<typename T>
void perturb(FPType percentage, FPType probOfPerturbation, std::vector<T>& container, int size) {
    assert(percentage > 0 && percentage <= 100 && 
		probOfPerturbation > 0  && probOfPerturbation <= 100);
	FPType bound = 0.0;
	FPType perturbation = 0.0;
	
	for (int index = 0; index < size; ++index) {
		if (rand() % 101 < probOfPerturbation) {
			bound = container[index] * percentage / 100.0;
			perturbation = -bound + static_cast <FPType> (rand()) /
				( static_cast <FPType> (RAND_MAX/(2 * bound)));
			container[index] += perturbation;
		}
	}

	std::sort(container.begin(), container.end());
}

void PieceWiseLinearIndiffCurve::perturbTime(FPType percentage, FPType probOfPerturbation){
	perturb(percentage, probOfPerturbation, values_, size_);
};

void PieceWiseLinearIndiffCurve::perturbToll(FPType percentage, FPType probOfPerturbation) {
	perturb(percentage, probOfPerturbation, tolls_, size_);
	std::sort(values_.begin(), values_.end());
};

void PieceWiseLinearIndiffCurve::restoreOriginalCurve(){
	for (int i = 0; i < size_; ++i) {
		tolls_[i] = tollsCopy_[i];
		values_[i] = valuesCopy_[i];
	}
};

