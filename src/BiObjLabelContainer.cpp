#include "BiObjLabelContainer.h"

#include <algorithm>
#include <cassert>
#include <iostream>

BiObjLabelContainer::BiObjLabelContainer(int nbNodes) : nbNodes_(nbNodes), labels_(nbNodes) {

};

BiObjLabelContainer::~BiObjLabelContainer(){
	deleteAllLabels();
};

LabelsIterator BiObjLabelContainer::begin(int nodeIndex)  const{
	assert(nodeIndex >= 0 && nodeIndex < nbNodes_);
	return labels_[nodeIndex].begin();
};

LabelsIterator BiObjLabelContainer::end(int nodeIndex)  const{
	assert(nodeIndex >= 0 && nodeIndex < nbNodes_);
	return labels_[nodeIndex].end();
};

LabelsIterator BiObjLabelContainer::erase(int nodeIndex, LabelsIterator& it){
	return labels_[nodeIndex].erase(it);
};

void BiObjLabelContainer::pushBackLabel(int nodeIndex, BiObjLabel* label) {
	labels_[nodeIndex].push_back(label);
};

void BiObjLabelContainer::deleteAllLabels(){
	for (int i = 0; i < nbNodes_; ++i){
		for (LabelsIterator labelIt = begin(i); labelIt != end(i); ++labelIt){
			delete *labelIt;
		}
		clearNodeLabels(i);
	}
};

void BiObjLabelContainer::clearNodeLabels(int nodeIndex){
	assert(nodeIndex >= 0 && nodeIndex < nbNodes_);
	labels_[nodeIndex].clear();
};

int BiObjLabelContainer::getNbLabels(int nodeIndex) const{
	assert(nodeIndex >= 0 && nodeIndex < nbNodes_);
	return labels_[nodeIndex].size();
};

bool BiObjLabelContainer::empty(int nodeIndex) const {
	return labels_[nodeIndex].empty();
};

void BiObjLabelContainer::print() const {
	for (int i = 0; i < nbNodes_; ++i){
		std::cout << "node index: " << i << std::endl;
		for (LabelsIterator labelIt = begin(i); labelIt != end(i); ++labelIt){
			std::cout << (*labelIt)->toString() << std::endl;
		}
	}
};