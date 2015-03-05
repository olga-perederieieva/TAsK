#include "BiDirectionalPoint2PointBSP.h"
#include "ReverseNetwork.h"
#include "BoundsCalculatorForBackwardNet.h"
#include "UtilsForDomination.h"
#include "DominationByPathCostBase.h"

#include <cassert>

BiDirectionalPoint2PointBSP::BiDirectionalPoint2PointBSP(StarNetwork& net,
					const TollContainerType& tolls, 
					BoundsCalculatorForBSPBase& bounds,
					const PathsAdder& pathAdder,
					BoundsCalculatorForBackwardNet& backwardBounds,
					DominationByPathCostBase& dominanceRule) :
					
					LabelSettingPoint2PointBSP(net, tolls, bounds, pathAdder, 
						dominanceRule), 
					forwardList_(NULL),
					backwardLabels_(net.getNbNodes()), forwardLabels_(NULL),
					curDirectionIsBackward_(true), backwardOriginIndex_(-1),
					backwardBounds_(backwardBounds) {
	ReverseNetwork createNet;
	reversedNet_ = createNet.createReverseStar(net); 
};

BiDirectionalPoint2PointBSP::~BiDirectionalPoint2PointBSP() {
	delete reversedNet_;
};

QueueType* BiDirectionalPoint2PointBSP::initLabels(BiObjLabel* originLabel,
										BiObjLabelContainer& labels){
	curDirectionIsBackward_ = true;
	int destIndex = getDestIndex();
	BiObjLabel* destLabel = new BiObjLabel(0, 0, destIndex, NULL, NULL);
	
	/*add destination label to backward list*/
	backwardList_.clear();
	backwardList_.insert(destLabel);
	
	/*add destination label to backward labels*/
	backwardLabels_.deleteAllLabels();
	backwardLabels_.pushBackLabel(destIndex, destLabel);
	
	forwardLabels_ = &labels;
	forwardList_ = LabelSettingBSPBase::initLabels(originLabel, labels);

	backwardOriginIndex_ = originLabel->getOwner();
	
	return forwardList_;
};

bool BiDirectionalPoint2PointBSP::stillLookingForLabels(const QueueType& tmpLabels) const{

	if (tmpLabels.empty() || backwardList_.empty()) {
		return false;
	};

	if (!forwardLabels_->empty(getDestIndex())) {

		QueueType::iterator lexMinForward = tmpLabels.begin();
		BiObjLabel* returnValForward = *lexMinForward;

		QueueType::iterator lexMinBackward = backwardList_.begin();
		BiObjLabel* returnValBackward = *lexMinBackward;	
		
		TollType newToll = 0; 
		FPType newTime = returnValForward->getTime() + returnValBackward->getTime(); //0 ;
		
		return !UtilsForDomination::isDominatedByLabelInDestNode(*forwardLabels_, getDestIndex(),
					newTime, newToll);
	}

	return true;
};

QueueType* BiDirectionalPoint2PointBSP::changeDirectionIfNeeded(){
	
	if (curDirectionIsBackward_ == false) {
		curDirectionIsBackward_ = true;
		return &backwardList_;
	}
	
	curDirectionIsBackward_ = false;
	return forwardList_;
};

BiObjLabelContainer* BiDirectionalPoint2PointBSP::changeLabelsIfNeeded(BiObjLabelContainer* labels){
	if (curDirectionIsBackward_ == true) {
		return &backwardLabels_;
	}
	return forwardLabels_;
};

StarNode* BiDirectionalPoint2PointBSP::beginNode(int owner) const {
	if (curDirectionIsBackward_ == true)  return reversedNet_->beginNode(owner);
	return net_.beginNode(owner);
};

StarLink* BiDirectionalPoint2PointBSP::beginLink() const {
	if (curDirectionIsBackward_ == true) return reversedNet_->beginLink();
	return net_.beginLink();
};

StarLink* BiDirectionalPoint2PointBSP::getNextLink() const {
	if (curDirectionIsBackward_ == true) return reversedNet_->getNextLink();
	return net_.getNextLink();
};

bool BiDirectionalPoint2PointBSP::isWorthAdding(FPType newTime, TollType newToll,
												int nodeIndex) const {

	if (curDirectionIsBackward_ == false) {
		return LabelSettingPoint2PointBSP::isWorthAdding(newTime, newToll, nodeIndex);
	}
	
	FPType timeLowerBound = newTime + backwardBounds_.getTimeLowerBound(nodeIndex, 
								backwardOriginIndex_);
	TollType tollLowerBound = newToll + backwardBounds_.getTollLowerBound(nodeIndex, 
								backwardOriginIndex_);
	
	return isWorth(timeLowerBound, tollLowerBound);	
};

void BiDirectionalPoint2PointBSP::addLabelToLabelSet(BiObjLabelContainer& labels, 
			int nodeIndex, 
			BiObjLabel* label) {
	if (curDirectionIsBackward_ == false) {
		dominanceRule_.addLabelToLabelSet(labels, nodeIndex, label, getDestIndex(),
					 currOdIndex_);
	} else {
		dominanceRule_.addLabelToLabelSet(labels, nodeIndex, label, backwardOriginIndex_,
					 currOdIndex_);
	}
};

void BiDirectionalPoint2PointBSP::combine(BiObjLabel* newLabel){
	
	BiObjLabelContainer* currentLabels = forwardLabels_;
	int destIndex = getDestIndex(); 
	int ownerInCurrLabel = newLabel->getOwner();
	if (curDirectionIsBackward_ == false) {
		currentLabels = &backwardLabels_;
	}
	
	if (ownerInCurrLabel != destIndex && !currentLabels->empty(ownerInCurrLabel)) {
		int forwardDestIndex = getDestIndex();
		for (LabelsIterator labelIt = currentLabels->begin(ownerInCurrLabel); 
						labelIt != currentLabels->end(ownerInCurrLabel); ++labelIt) {
			
			TollType newToll = newLabel->getToll() + (*labelIt)->getToll();
			FPType newTime = newLabel->getTime() + (*labelIt)->getTime();
			
			if (isNonDominated(newTime, newToll, (*forwardLabels_),	forwardDestIndex)) {
				BiObjLabel* tmpResult = NULL;
				if (curDirectionIsBackward_ == false) {
					tmpResult = new BiObjLabel(newTime, newToll, forwardDestIndex, newLabel, NULL);
				} else {
					tmpResult = new BiObjLabel(newTime, newToll, forwardDestIndex, (*labelIt), NULL);
				}
			
				mergeLabels(tmpResult, (*forwardLabels_), forwardDestIndex, (*forwardList_));
			} 

		}
	}
	
};

void BiDirectionalPoint2PointBSP::createPath(BiObjLabel *labelWithMinCost, std::list<StarLink*>& path) {
	
	BiObjLabel* label = labelWithMinCost;
	std::list<StarLink*> reversePathToDest;

	if (label->getPrevLink() == NULL && label->getPrevLabel() != NULL) {
		bool labelFound = false;
		int nodeIndex = reversedNet_->getNodeIndex(label->getPrevLabel()->getPrevLink()->getNodeTo());
		for (LabelsIterator it = backwardLabels_.begin(nodeIndex); 
							it != backwardLabels_.end(nodeIndex); ++it) {
			BiObjLabel* backwardLabel = *it;
			if (label->getPrevLabel()->getToll() + backwardLabel->getToll() == label->getToll() && 
					label->getPrevLabel()->getTime( )+ backwardLabel->getTime() == label->getTime()) {
				LabelSettingPoint2PointBSP::createPath(backwardLabel, reversePathToDest);
				labelFound = true;
				break;
			}
		}
		assert(labelFound);
	} else {
		path.push_back(label->getPrevLink());
	}
	
	for (std::list<StarLink*>::reverse_iterator rit=reversePathToDest.rbegin(); 
				rit!=reversePathToDest.rend(); ++rit) {
		path.push_back((*rit)->getForwardLink());
	}

	LabelSettingPoint2PointBSP::createPath(label->getPrevLabel(), path);
};
