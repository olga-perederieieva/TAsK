#include "OriginBushLUCE.h"
#include "DAGraphLUCE.h"

#include <stdlib.h>

LineSearch* OriginBushLUCE::lineSearch_ = NULL;

OriginBushLUCE::OriginBushLUCE(LineSearch* lineSearch) : 
				OriginBush(), dag_(NULL) {
	lineSearch_ = lineSearch;
}; 

OriginBushLUCE::~OriginBushLUCE() {

}; 

bool OriginBushLUCE::improve(){
	bool wasImproved = OriginBush::improve();
	dag_->prepareData();
	return wasImproved;
};

DAGraph* OriginBushLUCE::createNewDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow,
									  FPType dirTol){
	dag_ = new DAGraphLUCE(net, mat, zeroFlow, dirTol, index, lineSearch_); 
	return dag_;
};

