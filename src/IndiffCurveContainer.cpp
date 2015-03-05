#include "IndiffCurveContainer.h"
#include "PieceWiseLinearIndiffCurve.h"
#include "Utils.h"
#include "ODMatrix.h"
#include "OneSourceBiObjShPath.h"
#include "Origin.h"
#include "FileWriter.h"
#include "PairOD.h"

#include <stdio.h>      
#include <stdlib.h> 
#include <cassert>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <math.h>
#include <sstream>
#include <limits>

IndiffCurveContainer::IndiffCurveContainer(int nbODpairs) : curves_(nbODpairs) {
	for (int i = 0; i < nbODpairs; ++i){
		curves_[i] = NULL;
	}
}; 

IndiffCurveContainer::~IndiffCurveContainer() {
	deallocateCurves();
}; 

void IndiffCurveContainer::addCurve(int odPairIndex, IndiffCurve* curve) {
	assert(odPairIndex >= 0 && static_cast<size_t>(odPairIndex) < curves_.size() 
				&& curves_[odPairIndex] == NULL && curve != NULL);
	curves_[odPairIndex] = curve;
}; 

IndiffCurve* IndiffCurveContainer::getCurve(int odPairIndex) const {
	assert(odPairIndex >=0 && static_cast<size_t>(odPairIndex) < curves_.size() 
		&& curves_[odPairIndex] != NULL);
	return curves_[odPairIndex];
}; 

void IndiffCurveContainer::generateRandomCurvesWithBSP(int maxNbPointsPerCurve, 
				OneSourceBiObjShPath& shPath,
				const ODMatrix& mat){
	deallocateCurves();
	for (ConstOriginIterator it = mat.begin(); it != mat.end(); ++it){
		Origin* origin = *it;
		shPath.calculate(origin->getIndex());
		
		const BiObjLabelContainer& labels = shPath.getLabels();
		int maxNbLabels = 0;
		for (PairODIterator destIt = origin->begin(); destIt != origin->end(); ++destIt) {
			PairOD* dest = *destIt;
			int maxToll = 0;
			FPType maxTime = 0;
			FPType minTime = std::numeric_limits<FPType>::infinity();
			int destIndex = dest->getIndex();
			if (labels.getNbLabels(destIndex) > maxNbLabels)
				 maxNbLabels = labels.getNbLabels(destIndex);
			for (LabelsIterator labelIt = labels.begin(destIndex);
							labelIt != labels.end(destIndex); ++labelIt ) {
				BiObjLabel* label = *labelIt;
				if (label->getToll() > maxToll) {
					maxToll = label->getToll();
				}
				if (label->getTime() > maxTime) {
					maxTime = label->getTime();
				}
				if (label->getTime() < minTime) {
					minTime = label->getTime();
				}
			}
			curves_[dest->getODIndex()] = createCurve(std::min(maxNbPointsPerCurve, maxToll + 1), 
				maxToll + 1, 3*maxTime, minTime); 
		}
		
	}
};

void IndiffCurveContainer::generateRandomCurves(int maxNbPointsPerCurve, TollType maxToll,
						FPType maxTime){
	deallocateCurves();
	for (size_t i = 0; i < curves_.size(); ++i){
		curves_[i] = IndiffCurveContainer::createCurve(maxNbPointsPerCurve, maxToll, maxTime, 0);
	}
};

IndiffCurve* IndiffCurveContainer::createCurve(int maxNbPointsPerCurve, TollType maxToll,
						FPType maxTime, FPType minTime){
	assert(maxToll > 0 && maxNbPointsPerCurve <= maxToll && maxTime > 0);
	int nbPoints = rand() % maxNbPointsPerCurve + 1;
	std::vector<FPType> randomTimes(nbPoints);
	std::set<TollType> randomTolls;
	generateRandomTolls(randomTolls, maxToll, nbPoints);
	for (int i = 0; i < nbPoints; ++i) {
		randomTimes[i] =  Utils::generateRndNumber(maxTime); 
	}
	std::sort(randomTimes.begin(), randomTimes.end());
	PieceWiseLinearIndiffCurve* curve = new PieceWiseLinearIndiffCurve(nbPoints);
	std::set<TollType>::iterator it = randomTolls.begin();
	for (int i = 0; i < nbPoints; ++i){
		assert(it != randomTolls.end());
		curve->addPoint(*it, randomTimes[i]);
		++it;
	}
	return curve;
};

void IndiffCurveContainer::generateRandomTolls(std::set<TollType>& randomTolls,
							TollType maxToll, int nbPoints) {
	randomTolls.insert(0);
	size_t size = nbPoints;
	while (randomTolls.size() != size) {
		randomTolls.insert(rand() % maxToll + 1);
	}
};

void IndiffCurveContainer::deallocateCurves() {
	for (size_t i = 0; i < curves_.size(); ++i){
		delete curves_[i];
	}
}; 

void IndiffCurveContainer::print() const{
	for (size_t i = 0; i < curves_.size(); ++i){
		std::cout << "O-D pair " << i << " curve: " << (curves_[i])->toString() << std::endl;
	}
};

void IndiffCurveContainer::writeToFile(const std::string& fileName, const ODMatrix& mat) const {
	FileWriter writer(fileName);
	writer.writeLine("// Origin index    Destination index     OD index    Curve   \n");
	for (ConstOriginIterator it = mat.begin(); it != mat.end(); ++it){
		Origin* origin = *it;
		for (PairODIterator destIt = origin->begin(); destIt != origin->end(); ++destIt) {
			std::stringstream ss;
			PairOD* dest = *destIt;
			ss << origin->getIndex() << " " << dest->getIndex() << " " << dest->getODIndex() << 
				" " << (curves_[dest->getODIndex()])->toString() << "\n";
			writer.writeLine(ss.str());
		}
	}
};