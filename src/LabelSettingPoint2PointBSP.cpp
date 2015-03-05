#include "Error.h"
#include "LabelSettingPoint2PointBSP.h"
#include "PathsAdder.h"
#include "UtilsForDomination.h"
#include "DominationByPathCostBase.h"

#include <limits>
#include <algorithm>

LabelSettingPoint2PointBSP::LabelSettingPoint2PointBSP(StarNetwork& net, 
	const TollContainerType& tolls, BoundsCalculatorForBSPBase& bounds,
	 const PathsAdder& pathAdder,
	 DominationByPathCostBase& dominanceRule):
	Point2PointBiObjShPath(net.getNbNodes()), 
	LabelSettingBSPBase(net, tolls), 
	currOdIndex_(-1), dominanceRule_(dominanceRule),
	destNode_(-1), bounds_(bounds), 
	pathAdder_(pathAdder){
};

LabelSettingPoint2PointBSP::~LabelSettingPoint2PointBSP(){
};

void LabelSettingPoint2PointBSP::calculate(int originIndex, int destIndex, int odIndex){
	destNode_ = destIndex;
	currOdIndex_ = odIndex;

	dominanceRule_.resetBestKnownPathCost();
	bounds_.updateTimeBounds(destIndex, odIndex);

	calculateBSP(originIndex, labels_);
};

void LabelSettingPoint2PointBSP::addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
			BiObjLabel* label) {
	dominanceRule_.addLabelToLabelSet(labels, nodeIndex, label, destNode_, currOdIndex_);
};

void LabelSettingPoint2PointBSP::uploadPaths() {
	pathAdder_.uploadPaths(currOdIndex_, destNode_, labels_);
	dominanceRule_.updateBestKnown(labels_, destNode_, currOdIndex_);
};

void LabelSettingPoint2PointBSP::createPath(BiObjLabel *labelWithMinCost, 
	std::list<StarLink*>& path){
	UtilsForDomination::createPathFromLabel(labelWithMinCost, path);
};

bool LabelSettingPoint2PointBSP::stillLookingForLabels(const QueueType& tmpLabels) const{
	// Important: it is assumed that tmpLabels are sorted lexicographically
	if (tmpLabels.empty()) return false;
	FPType minTime = (*tmpLabels.begin())->getTime();
	TollType minToll = 0; // usually we always have a label with zero toll,
	// // even if we do not have it, it is a proper lower bound
	return !UtilsForDomination::isDominatedByLabelInDestNode(labels_, destNode_, minTime, minToll);
	// this strategy does seem to speed up calculations
	// return dominanceRule_.stillLookingForLabels(minTime, minToll, currOdIndex_);
};

bool LabelSettingPoint2PointBSP::isWorthAdding(FPType newTime, TollType newToll, int nodeIndex) const {
	FPType timeLowerBound = newTime + bounds_.getTimeLowerBound(nodeIndex, destNode_);
	TollType tollLowerBound = newToll + bounds_.getTollLowerBound(nodeIndex, destNode_);
	return isWorth(timeLowerBound, tollLowerBound);
};

bool LabelSettingPoint2PointBSP::isWorth(FPType timeLowerBound, TollType tollLowerBound) const {
	return dominanceRule_.isWorth(labels_, destNode_, timeLowerBound, 
				tollLowerBound, currOdIndex_);
};

int LabelSettingPoint2PointBSP::getDestIndex() const{
	return destNode_;
};

