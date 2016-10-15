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

	FPType numerator = 0.0;
    FPType denominator = 0.0;
    for (int i = 0; i < nbLinks_; ++i) {
      	FPType const dkfw = linkFlowsAux_[i] - linkFlows_[i];          // d_k^FW = y_k^FW - x_k
    	FPType const dkMinus1_bar = linkFlowsCFW_[i] - linkFlows_[i];  // d_{k-1}^BAR = s_{k-1}^CFW  - x_k
    	FPType const hk = net_->getLink(i)->getDerivative();
   		numerator += dkMinus1_bar * hk * dkfw;
    	denominator += dkMinus1_bar * hk * (dkfw - dkMinus1_bar);
  	}
  	FPType const val = numerator / denominator;
	if (val > 1.0 - zeroFlow_) return 1.0 - zeroFlow_;
	if (val >= 0.0) return val; 
	return 0.0;	
};

