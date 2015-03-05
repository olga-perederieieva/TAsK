#include "PASManagerWithStep.h"
#include "PASWithStep.h"

PASManagerWithStep::PASManagerWithStep(const ShortestPath &shPath, FPType dirTol, int nbNodes, FPType mu, 
					FPType v, int nbLinks, LineSearch* lineSearch, 
					FPType zeroFlow) : 
					PASManager(shPath, dirTol, nbNodes, mu, v, zeroFlow), 
					lineSearch_(lineSearch), 
					nbLinks_(nbLinks) {

};

PASManagerWithStep::~PASManagerWithStep(){

};

PAS* PASManagerWithStep::allocatePAS(){
	return new PASWithStep(zeroFlow_, dirTol_, lineSearch_, nbLinks_);
};