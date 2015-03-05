#include "ShortestPathWithRandomReturnWithFixedProbability.h"

ShortestPathWithRandomReturnWithFixedProbability::
			ShortestPathWithRandomReturnWithFixedProbability(ShortestPath* shPath, FPType probability):
			ShortestPathWithRandomReturn(shPath), probability_(probability) {

};

ShortestPathWithRandomReturnWithFixedProbability::~ShortestPathWithRandomReturnWithFixedProbability() {

};

FPType ShortestPathWithRandomReturnWithFixedProbability::getProbability() const {
	return probability_;
};