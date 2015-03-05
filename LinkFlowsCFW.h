#ifndef LINK_FLOWS_CFW
#define LINK_FLOWS_CFW

#include "LinkFlows.h"

/** \brief This class implements conjugate Frank-Wolf algorithm.
	\details For details see \cite Mitradjieva2012.
	\warning This algorithm shows problems with convergence when precision is high.
	Also for some reason it does not converge when Armijo step is used with decrement 2
	on Barcelona instance.
*/
class LinkFlowsCFW : public LinkFlows {
	
	public:
		LinkFlowsCFW(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, LineSearch* lineSearch, 
			FPType zeroFlow, LabelCorrectingAl* LCShPath);
		virtual ~LinkFlowsCFW();
		
	protected:
		
		FPType *linkFlowsCFW_; /**< CFW link flows. */
		int nbCalls_; /**< number of calculations of direction of descent,
							it is needed for initial iteration.*/
		
		virtual void calculateDirection();
		
	private:
		/** This method calculates a specific parameter of CFW, see \cite Mitradjieva2012.
		*/
		FPType calculateAlpha();
};

#endif
