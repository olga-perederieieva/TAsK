#include "KMLSelectLink.h"
#include "PathSet.h"
#include "StarNetwork.h"
#include "Path.h"
#include "ODSet.h"

#include <sstream>

KMLSelectLink::KMLSelectLink(StarNetwork* net, const PathSet& paths) :
			KMLNetOutput(net), currentLiknIndex_(-1), paths_(paths), 
			coloredLinks_(net->getNbLinks(), ""), 
			linkDescriptions_(net->getNbLinks(), 0) {
	initialiseColoredLinks();
};

KMLSelectLink::KMLSelectLink(StarNetwork* net, const PathSet& paths, 
			 			const std::string& logFileForMissingIDs) :
			KMLNetOutput(net, logFileForMissingIDs), currentLiknIndex_(-1), paths_(paths),
			coloredLinks_(net->getNbLinks(), ""),
			linkDescriptions_(net->getNbLinks(), 0) {
	initialiseColoredLinks();
};

KMLSelectLink::~KMLSelectLink() {

};

void KMLSelectLink::initialiseColoredLinks() {
	for (size_t i = 0; i < coloredLinks_.size(); ++i) {
		coloredLinks_[i] = getBlueColor();
	}
};

void KMLSelectLink::setLinkIndex(int linkIndex) {
	assert(linkIndex >= 0);
	currentLiknIndex_ = linkIndex;
	colorLinks();
};

FPType KMLSelectLink::calcLineWidth(StarLink* link) {
	return link->getIndex(); // link index will define link color
};

std::string KMLSelectLink::createColorFromLineWidth(FPType lineWidth) {
	return coloredLinks_[static_cast<int>(lineWidth)];
};

std::string KMLSelectLink::createLineWidth(FPType lineWidth) { 
	if (coloredLinks_[static_cast<int>(lineWidth)] == getRedColor()) return "5";
	return "3"; 
};

bool KMLSelectLink::shouldCreatePlacemark(StarLink* link) {
	return true;
};

std::string KMLSelectLink::createDescriptionString(StarLink* link) {
	std::stringstream ss;
	ss << "is present in " << linkDescriptions_[link->getIndex()] << " paths";
	return ss.str();
};

void KMLSelectLink::colorLinks() {
	std::stringstream ss;
	for (ConstODSetIterator odsetIt = paths_.begin(); odsetIt != paths_.end(); ++odsetIt) {
		ODSet* odset = *odsetIt;
		for (PathIterator pathIt = odset->begin(); pathIt != odset->end(); ++pathIt) {
			Path* path = *pathIt;
			if (pathHasSelectedLink(path)) {
				colorLinksOfPath(path, ss.str());
			}
		}
	}
};

bool KMLSelectLink::pathHasSelectedLink(Path* path) const {
	for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
		if ((*it)->getIndex() == currentLiknIndex_) {
			return true;
		}
	}
	return false;
};

void KMLSelectLink::colorLinksOfPath(Path* path, const std::string& decription) {
	for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
		StarLink* link = *it;
		coloredLinks_[link->getIndex()] = getRedColor();
		++linkDescriptions_[link->getIndex()];
	}
};
