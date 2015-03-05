#include "LinkFlowsCFW.h"
#include "StarNetwork.h"

#include <math.h>

LinkFlowsCFW::LinkFlowsCFW(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, 
		LineSearch* lineSearch, FPType zeroFlow, LabelCorrectingAl* LCShPath) : 
		LinkFlows(net, mat, shPath, lineSearch, zeroFlow, LCShPath), nbCalls_(0) {
	linkFlowsCFW_ = new FPType[nbLinks_];
	for (int i = 0; i < nbLinks_; ++i) {
		linkFlowsCFW_[i] = 0.0;
	}
};

LinkFlowsCFW::~LinkFlowsCFW(){
	delete[] linkFlowsCFW_;
};


void LinkFlowsCFW::calculateDirection(){
	
	calculateFWAux();
	
	FPType alpha = 0.0;
	if (nbCalls_ > 0) alpha = calculateAlpha();
	FPType beta = 1.0 - alpha;
	
	for (int i = 0; i < nbLinks_; ++i) {
		linkFlowsCFW_[i] = alpha * linkFlowsCFW_[i] + beta * linkFlowsAux_[i];
		direction_[i] = linkFlowsCFW_[i] - linkFlows_[i];
	}
	++nbCalls_;
	
};

FPType LinkFlowsCFW::calculateAlpha(){
	FPType nk = 0.0;
	FPType tmp = 0.0;
	FPType dk_tmp = 0.0;
	FPType tmp2 = 0.0;
	for (int i = 0; i < nbLinks_; ++i) {
		tmp2 = linkFlowsAux_[i] - linkFlows_[i];
		tmp =  tmp2 * (net_->getLink(i))->getDerivative();
		nk +=  (linkFlowsCFW_[i] - linkFlows_[i]) * tmp;
		dk_tmp += tmp2 * tmp;
	}
	dk_tmp = nk - dk_tmp;
	FPType val = nk / dk_tmp;
	if (fabs(dk_tmp) < zeroFlow_) return 0.0; 
	if (val > 1.0 - zeroFlow_) return 1.0 - zeroFlow_;
	if (val >= 0.0) return val; 
	return 0.0;
	
};

