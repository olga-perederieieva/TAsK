#include "UtilsForDomination.h"
#include "BiObjLabelContainer.h"

const FPType UtilsForDomination::tolerance_ = 1e-15;

bool UtilsForDomination::isDominated(BiObjLabel* first, BiObjLabel* second) {
	return (second->getTime() < first->getTime() + tolerance_  && 
			second->getToll() <= first->getToll());
};

bool UtilsForDomination::isDominated(FPType newTime, TollType newToll, BiObjLabel* second) {
	return (second->getTime() < newTime + tolerance_  && 
			second->getToll() <= newToll);
};

bool UtilsForDomination::isDominatedByLabelInDestNode(const BiObjLabelContainer& labels, 
			int destIndex, FPType newTime, TollType newToll) {
	for (LabelsIterator it = labels.begin(destIndex); it != labels.end(destIndex); ++it) {
		if (isDominated(newTime, newToll, *it)) {
			return true;
		}
	}
	return false;
};

void UtilsForDomination::createPathFromLabel(BiObjLabel* destLabel, std::list<StarLink*> &path) {
	BiObjLabel* label = destLabel;
	
	while (label != NULL) {
		if (label->getPrevLink() != NULL) path.push_back(label->getPrevLink());
		label = label->getPrevLabel();

	}
};