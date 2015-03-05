#include "KMLPlotTolls.h"
#include "StarLink.h"

#include <sstream>

KMLPlotTolls::KMLPlotTolls(StarNetwork* net, const TollContainerType& tolls) : 
		KMLNetOutput(net),
		tolls_(tolls) {

};

KMLPlotTolls::KMLPlotTolls(StarNetwork* net, const std::string& logFileForMissingIDs,
			const TollContainerType& tolls) :
			KMLNetOutput(net, logFileForMissingIDs),
			tolls_(tolls) {

};

KMLPlotTolls::~KMLPlotTolls() {

};

FPType KMLPlotTolls::calcLineWidth(StarLink* link) {
	if (tolls_[link->getIndex()] == 0) return -1;
	return 0;
};

std::string KMLPlotTolls::createColorFromLineWidth(FPType lineWidth) {
	if (lineWidth < 0) return getBlueColor();
	return getRedColor();
};

std::string KMLPlotTolls::createLineWidth(FPType lineWidth) {
	return "3";
};

bool KMLPlotTolls::shouldCreatePlacemark(StarLink* link) {
	return true;
};

std::string KMLPlotTolls::createDescriptionString(StarLink* link) {
	std::stringstream ss;
	ss << "Toll: " << tolls_[link->getIndex()];
	return ss.str();
};
