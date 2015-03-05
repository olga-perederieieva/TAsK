#include "BoundsCalculatorForBSP.h"
#include "ReverseNetworkWithTolls.h"
#include "StarNetwork.h"
#include "StarNode.h"

#include <cassert>

BoundsCalculatorForBSP::BoundsCalculatorForBSP(StarNetwork& net, const TollContainerType& tolls): 
			zeroFlowTimes_(net.getNbNodes() * net.getNbNodes(), 0),
			tolls_(net.getNbNodes() * net.getNbNodes(), 0),
			originalNet_(&net), linkTolls_(tolls) {
};

void BoundsCalculatorForBSP::initializeBounds() {
	reverseStar_ = getNetwork(*originalNet_);
	spp_ = new LabelCorrectingAlWithoutZonesCheck(reverseStar_);

	StarNetwork* netTolls = getNetworkWithTolls(*originalNet_, linkTolls_);
	LabelCorrectingAlWithoutZonesCheck sppWithTolls(netTolls);

    StarNetwork* oppositeNet = getOppositeNet();
    int nbNodes = oppositeNet->getNbNodes();

    StarNode* node = NULL;
	for(int i = 0; i < nbNodes; ++i){
    	node = reverseStar_->getNodeWithLinks(i);
    	if (node != NULL) {
	        spp_->calculate(i);
	        sppWithTolls.calculate(i);
	        
	        StarNode* tollNode = netTolls->getNodeWithLinks(i);
	        assert(tollNode != NULL);
	        assert(node->getID() == tollNode->getID());

	        int iForwardIndex = oppositeNet->getNodeIndex(node->getID());

	        for (int j = 0; j < nbNodes; ++j){
	            
	            int jID_rNet = reverseStar_->findID(j);
	            int jID_tollNet = netTolls->findID(j);
	           
	            assert(jID_tollNet == jID_rNet);  
	            if (jID_rNet != -1) {

		            int jForwardIndex = oppositeNet->getNodeIndex(jID_rNet);

		            int lineIndex = iForwardIndex*nbNodes + jForwardIndex; 
		            zeroFlowTimes_[lineIndex] = spp_->getCost(j); 
		            if (zeroFlowTimes_[lineIndex] == std::numeric_limits<FPType>::infinity()) {
		            	tolls_[lineIndex] = std::numeric_limits<TollType>::infinity();
		            } else {
			            tolls_[lineIndex] = sppWithTolls.getCost(j);
			        }
		            if (tolls_[lineIndex] < 0) {
		             	tolls_[lineIndex] = std::numeric_limits<TollType>::infinity();
		            }
		            assert(tolls_[lineIndex] >= 0);
		         } 
	        }
	    }
    }

    delete netTolls;
};

StarNetwork* BoundsCalculatorForBSP::getOppositeNet() {
	return originalNet_;
};

StarNetwork* BoundsCalculatorForBSP::getNetwork(StarNetwork& net) {
	ReverseNetwork rNetCreator;
	std::cout << "Creating reversed net for usual bound" << std::endl;
	return rNetCreator.createReverseStar(net);
};

StarNetwork* BoundsCalculatorForBSP::getNetworkWithTolls(StarNetwork& net, const TollContainerType& tolls) {
	ReverseNetworkWithTolls rNetCreatorTolls(tolls);
	std::cout << "Creating reversed net with tolls for usual bound" << std::endl;
	return rNetCreatorTolls.createReverseStar(net);
};

BoundsCalculatorForBSP::~BoundsCalculatorForBSP() {
	if (reverseStar_ != originalNet_) delete reverseStar_;
	delete spp_;
};

void BoundsCalculatorForBSP::updateTimeBounds(int destNodeIndex, int odPairIndex) {

};

TollType BoundsCalculatorForBSP::getTollLowerBound(int nodeIndex, int destNodeIndex) {
	return tolls_[destNodeIndex*reverseStar_->getNbNodes() + nodeIndex];
};

FPType BoundsCalculatorForBSP::getTimeLowerBound(int nodeIndex, int destNodeIndex) {
	return zeroFlowTimes_[destNodeIndex*reverseStar_->getNbNodes() + nodeIndex];
};