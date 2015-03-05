#include "LabelSettingBSPBase.h"
#include "Error.h"
#include "UtilsForDomination.h"
#include "LatexNetOutputLinksExploredInBSP.h"
#include "StarLink.h"

#include <list>
#include <algorithm> 
#include <sstream>
#include <cassert>

LabelSettingBSPBase::LabelSettingBSPBase(StarNetwork& net, const TollContainerType& tolls) : 
				net_(net), tolls_(tolls) 
				, seenLinks_(net.getNbLinks()) {
	for (int i = 0; i < net.getNbLinks(); ++i) {
		seenLinks_[i] = NULL;
	}
};

LabelSettingBSPBase::~LabelSettingBSPBase() {};

void LabelSettingBSPBase::calculateBSP(int originIndex, BiObjLabelContainer& labels) {
	
	BiObjLabel* originLabel = new BiObjLabel(0, 0, originIndex, NULL, NULL);

	BiObjLabelContainer* labelsPointer = &labels;

	labelsPointer->deleteAllLabels();

	uploadPaths();

	labelsPointer->pushBackLabel(originIndex, originLabel);
	
	QueueType* tmpLabels = initLabels(originLabel, labels);

	while (stillLookingForLabels(tmpLabels_)) { // stopping condition must always 
												// get original tmpLabels_

		tmpLabels = changeDirectionIfNeeded(); // might change tmpLabels_ to a 
											   // different set for bi-directional version
		labelsPointer = changeLabelsIfNeeded(labelsPointer);

		QueueType& tmpLabelsRef = *tmpLabels;
		
		BiObjLabel* currentLabel = getNextLabelAndRemoveItFromList(tmpLabelsRef);
		int owner = currentLabel->getOwner();
		
		StarNode* curNode = beginNode(owner);
		if ((curNode != NULL)) { 
			// && (!curNode->getIsZone() || (owner == originIndex))) { // special condition for zones
			for (StarLink* link = beginLink(); link != NULL; link = getNextLink()) { 
				
				assert(currentLabel != NULL);
				TollType newToll = currentLabel->getToll() + tolls_[link->getForwardLink()->getIndex()];
				FPType newTime = currentLabel->getTime() + link->getTime();
					
				int linkHead = link->getNodeToIndex();
				if ( isWorthAdding(newTime, newToll, linkHead) && 
						isNonDominated(newTime, newToll, *labelsPointer, linkHead)) {
					BiObjLabel* newLabel = 
								new BiObjLabel(newTime, newToll, linkHead, currentLabel, link);
					
					mergeLabels(newLabel, *labelsPointer, linkHead, tmpLabelsRef);
					tmpLabelsRef.insert(newLabel);
					combine(newLabel);
				
				} 


			}
		}

	};

};

void LabelSettingBSPBase::combine(BiObjLabel* newLabel) {

};

void LabelSettingBSPBase::uploadPaths() {

};

QueueType* LabelSettingBSPBase::initLabels(BiObjLabel* originLabel, BiObjLabelContainer& labels) {
	tmpLabels_.clear();
	tmpLabels_.insert(originLabel);
	return &tmpLabels_;
};

bool LabelSettingBSPBase::stillLookingForLabels(const QueueType& tmpLabels) const {
	return !tmpLabels.empty();
};

QueueType* LabelSettingBSPBase::changeDirectionIfNeeded() {
	return &tmpLabels_;
};

BiObjLabelContainer* LabelSettingBSPBase::changeLabelsIfNeeded(BiObjLabelContainer* labels) {
	return labels;
};

StarNode* LabelSettingBSPBase::beginNode(int owner) const {
	return net_.beginNode(owner);
};

StarLink* LabelSettingBSPBase::beginLink() const {
	return net_.beginLink();
};

StarLink* LabelSettingBSPBase::getNextLink() const {
	return net_.getNextLink();
};

BiObjLabel* LabelSettingBSPBase::getNextLabelAndRemoveItFromList(QueueType& tmpLabels) {
	QueueType::iterator lexMin = tmpLabels.begin();
	assert(lexMin != tmpLabels.end());
	BiObjLabel* returnVal = *lexMin;
	tmpLabels.erase(lexMin);
	return returnVal;
};

BiObjLabel* LabelSettingBSPBase::createLabel(BiObjLabel* currentLabel, StarLink* link) const {
	TollType newToll = currentLabel->getToll() + tolls_[link->getIndex()];
	FPType newTime = currentLabel->getTime() + link->getTime();
	return new BiObjLabel(newTime, newToll, link->getNodeToIndex(), currentLabel, link);
};

bool LabelSettingBSPBase::isNonDominated(FPType newTime, TollType newToll, const BiObjLabelContainer& labels, 
				int linkHead) const {
	for (LabelsIterator it = labels.begin(linkHead); it != labels.end(linkHead); ++it) {
		if (UtilsForDomination::isDominated(newTime, newToll, *it)) return false;
	}
	return true;
};

void LabelSettingBSPBase::mergeLabels(BiObjLabel* label, BiObjLabelContainer& labels, int linkHead,
				QueueType& tmpLabels) {
	for (LabelsIterator it = labels.begin(linkHead); it != labels.end(linkHead); ++it) {
		BiObjLabel* labelInNode = *it;
		if (UtilsForDomination::isDominated(labelInNode, label)) {
			
			deleteFromTmpLabels(tmpLabels, labelInNode);

			delete labelInNode;
			it = labels.erase(linkHead, it);
			--it;
		}
	}
	addLabelToLabelSet(labels, linkHead, label);
};

void LabelSettingBSPBase::addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex,
		BiObjLabel* label) {
	labels.pushBackLabel(nodeIndex, label);
};

void LabelSettingBSPBase::deleteFromTmpLabels(QueueType& tmpLabels, BiObjLabel* labelInNode){

	std::pair<QueueType::iterator, QueueType::iterator> equalValues = 
								tmpLabels.equal_range(labelInNode);
	QueueType::iterator elemToDelete = equalValues.first;
	for (; elemToDelete != equalValues.second; ++elemToDelete) {
	 	if (*elemToDelete == labelInNode){
	 		break;
	 	}
	}
	if (elemToDelete != equalValues.second) tmpLabels.erase(elemToDelete); 
};
		
