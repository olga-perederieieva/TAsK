#ifndef LABEL_CORRECTING_ONE_SOURCE_BSP
#define LABEL_CORRECTING_ONE_SOURCE_BSP 

#include "OneSourceBiObjShPath.h"
#include "LabelSettingBSPBase.h"

/** \brief Concrete implementation of the one-source bi-objective label-setting algorithm.
*/
class LabelSettingOneSourceBSP : public OneSourceBiObjShPath, private LabelSettingBSPBase {
	public:
		LabelSettingOneSourceBSP(StarNetwork& net, const TollContainerType& tolls);
		~LabelSettingOneSourceBSP();

		virtual void calculate(int originIndex);

};
#endif