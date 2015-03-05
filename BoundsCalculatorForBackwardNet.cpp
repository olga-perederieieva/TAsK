#include "BoundsCalculatorForBackwardNet.h"
#include "StarNetworkWithTolls.h"
#include "StarNetwork.h"

BoundsCalculatorForBackwardNet::BoundsCalculatorForBackwardNet(StarNetwork& net, 
			const TollContainerType& tolls) : 
			BoundsCalculatorForBSP(net, tolls),
			net_(&net), backwardNet_(NULL) {

};

BoundsCalculatorForBackwardNet::~BoundsCalculatorForBackwardNet() {
	delete backwardNet_;
};
		
StarNetwork* BoundsCalculatorForBackwardNet::getNetwork(StarNetwork& net) {
	backwardNet_ = BoundsCalculatorForBSP::getNetwork(net);
	std::cout << "Creating forward net for backward bound" << std::endl;
	return net_;
};

StarNetwork* BoundsCalculatorForBackwardNet::getNetworkWithTolls(StarNetwork& net,
						const TollContainerType& tolls) {
	
	StarNetworkWithTolls netWithTolls(tolls);
	std::cout << "Creating forward net for backward bound" << std::endl;
	return netWithTolls.createNetWithTolls(net);
};

StarNetwork* BoundsCalculatorForBackwardNet::getOppositeNet() {
	return backwardNet_;
};
