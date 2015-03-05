#ifndef FW_ALGO
#define FW_ALGO

#include "EqAlgo.h"

class LinkFlows;

/** \brief This class implements main steps of the framework for link-based algorithms.
	\details It is expected that there will be only one instance of this class.
 */
class FWAlgo : public EqAlgo {
	
	public:
	
		/** @param linkFlows object that contains concrete implementations for a given
				link-based algorithm.
			@param component object that performs extra operations with convergence
				information, see AddHook.
			@param net network.
			@param conv convergence measure.
			@param timeLimit time limit in seconds.
		*/
		FWAlgo(LinkFlows *linkFlows, AddHook *component, StarNetwork *net, ConvMeasure* conv,
					FPType timeLimit); 
		~FWAlgo();
		
	private:
		LinkFlows *linkFlows_;

		virtual void initialise();
		virtual void performOneIteration();
		virtual bool isConverged();
};

#endif
