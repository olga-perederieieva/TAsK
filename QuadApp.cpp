#include "QuadApp.h"
#include "StarNetwork.h"
#include "Derivative.h"
#include "StarLink.h"

#include <stdlib.h>

QuadApp::QuadApp(Derivative *der, StarNetwork* net) : 
					LineSearch(der), net_(net) {
	
};

QuadApp::~QuadApp(){

};

FPType QuadApp::execute(FPType a, FPType b){
	
	FPType sum1 = 0.0;
	FPType sum2 = 0.0;
	StarLink* link = NULL;
	Derivative &der = *der_;
	int size = der.getSize(); 
	FPType y = 0.0;
	for (int i = 0; i < size; ++i) {
		link = net_->getLink(der.getLinkIndex(i));
		y = der.getY(i);
		
		sum1 += link->getTime() * y;
		sum2 += link->getDerivative() * y * y;
	}
	if (sum1 >= 0.0) { 
		
		return 0.0;
	} 
	FPType returnVal = -sum1 / sum2;
	if (returnVal > b) returnVal = b;
	return returnVal;
};

