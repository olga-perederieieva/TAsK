#include "EqAlgo.h"
#include "AddHook.h"
#include "Timer.h"
#include "ConvMeasure.h"

#include <cassert>
#include <iostream>


EqAlgo::EqAlgo(AddHook *component, StarNetwork* net, FPType timeLimit, 
		ConvMeasure* conv) : 
		conv_(conv), net_(net), component_(component), timeLimit_(timeLimit), totalConvTime_(0.0),
		totalPureIterTime_(0.0), initialisationTime_(0.0) {
			assert(net_ != NULL);
			
		};


EqAlgo::~EqAlgo() {};
		
FPType EqAlgo::getTotalConvTime(){
	return totalConvTime_;
};

FPType EqAlgo::getTotalPureIterTime(){
	return totalPureIterTime_;
};

FPType EqAlgo::getInitialisationTime() {
	return initialisationTime_;
};

int EqAlgo::execute() {
	std::cout << "Initialisation" << std::endl;
	Timer initTimer;
	initTimer.start();
	initialise();
	initialisationTime_ = initTimer.getTimePassed();
	std::cout << "Initialisation done" << std::endl;
	
	int nbIter = 0; 
	Timer timer;
	FPType timePassed = 0.0;
	Timer convTimer;
	FPType convTime = 0.0;
	bool algoConverged = false;
	while (true) {
		
		++nbIter;
		
		// global convergence check
		convTimer.start();
		algoConverged = isConverged();
		convTime += convTimer.getTimePassed();
		if (algoConverged){
			break;
		}
		
		timer.start();
		performOneIteration();

		timePassed += timer.getTimePassed(); 

		// calculates the amount of link flow change
		// and other counters (it also stores them in string format). 
		component_->produceOut(timePassed, conv_->getGap());

		if (convTime + timePassed >= timeLimit_) {  
			convTimer.start();
			algoConverged = isConverged();
			convTime += convTimer.getTimePassed();
			break;
		}
	}
	component_->produceOut(timePassed, conv_->getGap());
	totalConvTime_ = convTime;
	totalPureIterTime_ = timePassed;
	return nbIter;
};

bool EqAlgo::isConverged(){
	return conv_->isConverged();
}
