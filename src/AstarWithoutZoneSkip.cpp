#include "AstarWithoutZoneSkip.h"

AstarWithoutZoneSkip::AstarWithoutZoneSkip(StarNetwork* net,
			ODMatrix* odMatrix) : Astar(net, odMatrix) {
};

AstarWithoutZoneSkip::~AstarWithoutZoneSkip() {

};

bool AstarWithoutZoneSkip::shouldSkipZone(StarNode* node, int nodeIndex, int originIndex) const {
	return false;
};