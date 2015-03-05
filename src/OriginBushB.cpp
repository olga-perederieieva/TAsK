#include "OriginBushB.h"
#include "DAGraphBWithStep.h"
#include "DAGraphB.h"

bool OriginBushB::useMultiStep_ = false;
LineSearch* OriginBushB::lineSearch_ = NULL;

OriginBushB::OriginBushB(bool useMultiStep, LineSearch* lineSearch) {
	useMultiStep_ = useMultiStep;
	lineSearch_ = lineSearch;
};

OriginBushB::~OriginBushB(){

};
		
DAGraph* OriginBushB::createNewDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow,
								   FPType dirTol){
	if (lineSearch_ == NULL) 
		return new DAGraphB(net, mat, zeroFlow, dirTol, index, useMultiStep_); 
	return new DAGraphBWithStep(net, mat, zeroFlow, dirTol, index, useMultiStep_, lineSearch_); 
};
