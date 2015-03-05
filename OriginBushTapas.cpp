#include "OriginBushTapas.h"
#include "DAGraphTapas.h"

#include <stdlib.h>

ShortestPath * OriginBushTapas::shPath_ = NULL;
PASManager * OriginBushTapas::pasManager_ = NULL;
		
OriginBushTapas::OriginBushTapas(ShortestPath *shPath, PASManager *pasManager) : dag_(NULL) {
	shPath_ = shPath;
	pasManager_ = pasManager;
};

OriginBushTapas::~OriginBushTapas(){

};
		
DAGraph* OriginBushTapas::createNewDAG(int index, StarNetwork *net, ODMatrix *mat,
									   FPType zeroFlow, FPType dirTol){
	dag_ = new DAGraphTapas(net, mat, zeroFlow, dirTol, index, shPath_, pasManager_); 
	return dag_; 
};

void OriginBushTapas::updateTopSort(){
	dag_->removeCyclicFlows();	
};
