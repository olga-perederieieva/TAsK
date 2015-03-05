#include "LinkFlowsBFW.h"
#include "StarNetwork.h"

#include <math.h>

LinkFlowsBFW::LinkFlowsBFW(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, LineSearch* lineSearch, 
		FPType zeroFlow, LabelCorrectingAl* LCShPath) : 
		LinkFlowsCFW(net, mat, shPath, lineSearch, zeroFlow, LCShPath) { 
	linkFlowsBFW_ = new FPType[nbLinks_];
	for (int i = 0; i < nbLinks_; ++i) {
		linkFlowsBFW_[i] = 0.0;
	}
};

LinkFlowsBFW::~LinkFlowsBFW(){
	delete[] linkFlowsBFW_;
};
		
	
void LinkFlowsBFW::calculateDirection(){
	
	if (nbCalls_ == 0) { // first iteration FW direction is used
		LinkFlows::calculateDirection();

		for (int i = 0; i < nbLinks_; ++i) {
			linkFlowsCFW_[i] =  linkFlowsAux_[i];
			linkFlowsBFW_[i] = 0.0;
		}

	} else if (nbCalls_ == 1) { // second iteration CFW direction is used
		for (int i = 0; i < nbLinks_; ++i) {
			linkFlowsBFW_[i] = linkFlowsCFW_[i];
		}
		LinkFlowsCFW::calculateDirection();

	} else {

		calculateFWAux();
		
		FPType beta0 = 0.0;
		FPType beta1 = 0.0;
		FPType beta2 = 0.0;
		calcCoeff(beta0, beta1, beta2);
		FPType promVal = 0.0;
		for (int i = 0; i < nbLinks_; ++i) {
			promVal = beta0 * linkFlowsAux_[i] + beta1 * linkFlowsCFW_[i] + beta2 * linkFlowsBFW_[i]; 
			linkFlowsBFW_[i] = linkFlowsCFW_[i];
			linkFlowsCFW_[i] = promVal;
			direction_[i] = linkFlowsCFW_[i] - linkFlows_[i];
		}
	}
	++nbCalls_;
	
};

void LinkFlowsBFW::calcCoeff(FPType &beta0, FPType &beta1, FPType &beta2){
	if ((fabs(1.0 - stepPrev_) <= zeroFlow_) || (fabs(1.0 - stepPrevPrev_) <= zeroFlow_) ) {
		beta0 = 1.0;
		beta1 = 0.0;
		beta2 = 0.0;
	} else {
		FPType nk = 0.0;
		FPType dk = 0.0;
		FPType nnk = 0.0;
		FPType ddk = 0.0;
		FPType der = 0.0;
		FPType dir_fw = 0.0;
		FPType dir_2 = 0.0;
		FPType dir_1 = 0.0;
		for (int i = 0; i < nbLinks_; ++i) {
			der = (net_->getLink(i))->getDerivative();
			dir_fw = linkFlowsAux_[i] - linkFlows_[i];
			dir_2 = stepPrev_ * linkFlowsCFW_[i] - linkFlows_[i] + (1 - stepPrev_) * linkFlowsBFW_[i];
			dir_1 = linkFlowsCFW_[i] - linkFlows_[i];
			nk += dir_2 * der * dir_fw;
			dk += dir_2 * der * (dir_2 - dir_1) / (1 - stepPrev_);
			nnk += dir_1 * der * dir_fw;
			ddk += dir_1 * dir_1 * der;
		}
		if ((fabs(dk) <= zeroFlow_) || (fabs(ddk) <= zeroFlow_) ) { 
			beta0 = 1.0;
			beta1 = 0.0;
			beta2 = 0.0;
		} else {
			FPType mu = -nk / dk;
			FPType eta = -nnk / ddk + (mu * stepPrev_) / (1.0 - stepPrev_);
			beta0 = 1.0 / (1.0 + mu + eta);
			if ((beta0 >= 0.0) && (beta0 <= 1.0 - zeroFlow_)) {
				beta1 = eta * beta0;
				beta2 = mu * beta0;
				
				if ((beta1 < 0.0) || (beta1 > 1.0 - zeroFlow_) || (beta2 < 0.0) || (beta2 > 1.0 - zeroFlow_)) {
					beta0 = 1.0;
					beta1 = 0.0;
					beta2 = 0.0;
				}
			} else {
				beta0 = 1.0;
				beta1 = 0.0;
				beta2 = 0.0;
			}
			
		}
	}
};
