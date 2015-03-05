#include "LatexNetOutputLinksExploredInBSP.h"
#include "BiObjLabelContainer.h"
#include "StarNetwork.h"
#include "StarLink.h"

#include <sstream>

LatexNetOutputLinksExploredInBSP::LatexNetOutputLinksExploredInBSP(StarNetwork* net, 
							const std::vector<StarLink*>& seenLinks,
							const BiObjLabelContainer& labels) : 
							LatexNetOutput(net), seenLinks_(seenLinks),
							labels_(labels) {

};

LatexNetOutputLinksExploredInBSP::LatexNetOutputLinksExploredInBSP(StarNetwork* net, 
							const std::vector<StarLink*>& seenLinks,
							const BiObjLabelContainer& labels,
			 				const std::string& logFileForMissingIDs) :
			 				LatexNetOutput(net, logFileForMissingIDs),
			 				seenLinks_(seenLinks), labels_(labels) {

};

LatexNetOutputLinksExploredInBSP::~LatexNetOutputLinksExploredInBSP() {

};

std::string LatexNetOutputLinksExploredInBSP::getLineLabel(StarLink* link, 
				const std::string& aboveOrBelow) {
	return "-- node[sloped," + aboveOrBelow + "]{}";
};

std::string LatexNetOutputLinksExploredInBSP::generateLineOptions(const FlowInfo& flowInfo, 
		StarLink* link) {
	if (seenLinks_[link->getIndex()] != NULL) {
		return "->, red";
	}
	return "->, blue";
};

bool LatexNetOutputLinksExploredInBSP::plotLink(StarLink* link) const {
	return true;
};

std::string LatexNetOutputLinksExploredInBSP::getNodeLabel(int nodeID) const {
	std::stringstream ss;
	ss << nodeID << ", " << labels_.getNbLabels(net_->getNodeIndex(nodeID));
	return ss.str();
};