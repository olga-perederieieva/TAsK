#include "DiffKMLNetOutput.h"
#include "StarLink.h"
#include "StarNetwork.h"

#include <sstream>
#include <limits>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <cassert>

DiffKMLNetOutput::DiffKMLNetOutput(StarNetwork* net, const std::vector<FPType>& flowDiff,
					FPType tolerance) : 
					KMLNetOutput(net), flowDiff_(flowDiff), scaleFactor_(1),
					tolerance_(tolerance) {
	calcScalingFactor();
};

void DiffKMLNetOutput::calcScalingFactor(){
	FPType maxDiff = 0.0;
	FPType minDiff = std::numeric_limits<FPType>::infinity();
	for (size_t i = 0; i < flowDiff_.size(); ++i){
		FPType absDiff = fabs(flowDiff_[i]);
		if (absDiff > maxDiff) maxDiff = absDiff;
		if (absDiff < minDiff) minDiff = absDiff;
	}
	std::cout << "maxDiff = " << maxDiff << " minDiff = " << minDiff << std::endl;
	scaleFactor_ = maxDiff / maxLength_;
	assert(scaleFactor_ != 0);
};

DiffKMLNetOutput::DiffKMLNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs,
					const std::vector<FPType>& flowDiff, FPType tolerance) : 
					KMLNetOutput(net, logFileForMissingIDs),
					flowDiff_(flowDiff), tolerance_(tolerance) {

};

DiffKMLNetOutput::~DiffKMLNetOutput(){

};

FPType DiffKMLNetOutput::calcLineWidth(StarLink* link){
	return flowDiff_[link->getIndex()] / scaleFactor_;
};

std::string DiffKMLNetOutput::createColorFromLineWidth(FPType lineWidth){
	if (lineWidth == 0) return getGreenColor();
	if (lineWidth < 0.0) return "50F0C814";//getBlueColor();
	return "501478FF";//getRedColor();
};

std::string DiffKMLNetOutput::createLineWidth(FPType lineWidth){
	if (lineWidth == 0) lineWidth = 3;
	std::stringstream ss;
    ss << fabs(lineWidth);
    return ss.str();
};

bool DiffKMLNetOutput::shouldCreatePlacemark(StarLink* link) {
	return true;
};

std::string DiffKMLNetOutput::createDescriptionString(StarLink* link) {

	std::stringstream ss;
	ss << "difference: " << flowDiff_[link->getIndex()];
	return ss.str();
};