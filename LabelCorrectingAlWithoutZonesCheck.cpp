#include "LabelCorrectingAlWithoutZonesCheck.h"

LabelCorrectingAlWithoutZonesCheck::LabelCorrectingAlWithoutZonesCheck(StarNetwork *netPointer) :
					LabelCorrectingAl(netPointer) {

};

LabelCorrectingAlWithoutZonesCheck::~LabelCorrectingAlWithoutZonesCheck(){

};

bool LabelCorrectingAlWithoutZonesCheck::proceed(StarNode* curNode, int topNode) const {
	return (curNode != NULL);
};
