#include "OriginSet.h"
#include "OriginBushLUCE.h"
#include "OriginBushB.h"
#include "OriginBushTapas.h"
#include "Error.h"
#include "Origin.h"
#include "ODMatrix.h"
#include "StarNetwork.h"
#include "LabelCorrectingAl.h"

OriginSet::OriginSet(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, PASManager *pasManager, 
					LabelCorrectingAl* LCShPath, bool useMultiStep,
					LineSearch* lineSearch, OriginAlgoType algo) : 
					net_(net), nbOrigins_(mat->getNbOrigins()),
					bushes_(mat->getNbOrigins()), indexes_(net->getNbNodes(), -1),
					aon_(*mat, LCShPath, net->getNbNodes()){

	int i = 0;
	for (OriginIterator it = mat->begin(); it != mat->end(); ++it){
		Origin* origin = *it;
		if (algo == B) {
			bushes_[i] =  new OriginBushB(useMultiStep);
		} else if (algo == Bstep) {
			bushes_[i] = new OriginBushB(useMultiStep, lineSearch);
		} else if (algo == LUCE) {
			bushes_[i] = new OriginBushLUCE(lineSearch);
		} else if (algo == TAPAS) {
			bushes_[i] = new OriginBushTapas(LCShPath, pasManager);
		} else {
			throw Error("It is required to create an origin-based algorithm that is not implemented.");
		}
		bushes_[i]->allocateDAG(origin->getIndex(), net, mat, zeroFlow, dirTol);
		indexes_[origin->getIndex()] = i; 
		++i;
	}

};

OriginSet* OriginSet::createOriginSetB(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, bool useMultiStep, LabelCorrectingAl* LCShPath){
	return new OriginSet(mat, net, zeroFlow, dirTol, NULL, LCShPath, useMultiStep, NULL, B);
};
		
OriginSet* OriginSet::createOriginSetBWithStep(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
			FPType dirTol, LineSearch* lineSearch, 
			LabelCorrectingAl* LCShPath){
	return new OriginSet(mat, net, zeroFlow, dirTol, NULL, LCShPath, false, lineSearch, Bstep);
};

OriginSet* OriginSet::createOriginSetLUCE(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
			FPType dirTol, LineSearch* lineSearch, LabelCorrectingAl* LCShPath){
	return new OriginSet(mat, net, zeroFlow, dirTol, NULL, LCShPath, false, lineSearch, LUCE);
};
		
OriginSet* OriginSet::createOriginSetTAPAS(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
			FPType dirTol, PASManager *pasManager, LabelCorrectingAl* LCShPath){
	return new OriginSet(mat, net, zeroFlow, dirTol, pasManager, LCShPath, false, NULL, TAPAS);
};

OriginSet::~OriginSet(){
	for(int i = 0; i < nbOrigins_; ++i){
		delete bushes_[i];
	}
};
		
void OriginSet::initialise(){
	aon_.execute(this);
	net_->calculateLinkCosts();
	
};

OriginSetIterator OriginSet::begin(){
	return bushes_.begin();
};

OriginSetIterator OriginSet::end(){
	return bushes_.end();
};

void OriginSet::initialiseItself(StarLink* link, PairOD *dest, int originIndex){
	bushes_[indexes_[originIndex]]->addLinkAndUpdateFlow(link, dest);

};

void OriginSet::print(){
	for (OriginSetIterator bushIt = begin(); bushIt != end(); ++bushIt){
		(*bushIt)->print();
	}
};
