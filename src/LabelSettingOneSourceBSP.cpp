#include "LabelSettingOneSourceBSP.h"

#include <iostream>

LabelSettingOneSourceBSP::LabelSettingOneSourceBSP(StarNetwork& net, 
		const TollContainerType& tolls) : OneSourceBiObjShPath(net.getNbNodes()), 
		LabelSettingBSPBase(net, tolls) {

		std::cout << "LabelSettingOneSourceBSP is created" << std::endl;

};

LabelSettingOneSourceBSP::~LabelSettingOneSourceBSP() {

};

void LabelSettingOneSourceBSP::calculate(int originIndex){
	calculateBSP(originIndex, labels_);
};