#include "AlgoDecoratorWriteLinks.h"
#include "StarNetwork.h"

#include <iostream>

AlgoDecoratorWriteLinks::AlgoDecoratorWriteLinks(DecoratedEqAlgo *algo, StarNetwork *net, 
				const std::string &fileName, const std::string& dir) : 
				AlgoDecoratorBase(algo), net_(net), fileName_(fileName), dir_(dir) {

}; 

AlgoDecoratorWriteLinks::~AlgoDecoratorWriteLinks() {
}; 

int AlgoDecoratorWriteLinks::execute(){
	int nbIter = component_->execute();
	std::cout << "Writing link flows to file: " << fileName_ << std::endl;
	net_->printToFile(dir_ + fileName_); //
	return nbIter;
};

