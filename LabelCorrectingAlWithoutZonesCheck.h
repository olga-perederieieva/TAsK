#ifndef LABEL_CORRECTING_ALGO_WITHOUT_ZONES_CHECK
#define LABEL_CORRECTING_ALGO_WITHOUT_ZONES_CHECK 

#include "LabelCorrectingAl.h"

/** \brief Implements usual one-source label-setting when shortest paths are
	allowed to go through zone nodes.
*/
class LabelCorrectingAlWithoutZonesCheck : public LabelCorrectingAl {
	public:
		LabelCorrectingAlWithoutZonesCheck(StarNetwork *netPointer);
		~LabelCorrectingAlWithoutZonesCheck();

	protected:
		/** @return true if curNode is not NULL, false otherwise.
		*/
		virtual bool proceed(StarNode* curNode, int topNode) const;

};

#endif