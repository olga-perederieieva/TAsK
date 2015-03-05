#include "AlgoDecorator.h"
#include "AddHookStoreOut.h"

AlgoDecorator::AlgoDecorator(DecoratedEqAlgo *algo, AddHookStoreOut *hook, const std::string& fileName,
					const std::string& dir) : AlgoDecoratorBase(algo),
					hook_(hook), fileName_(fileName), dir_(dir) {

};

AlgoDecorator::~AlgoDecorator(){
};

int AlgoDecorator::execute(){
	int nbIter = component_->execute();
	hook_->printToFile(dir_ + fileName_);
	return nbIter;
};
