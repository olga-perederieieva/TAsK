#ifndef LINK_FLOWS_BFW
#define LINK_FLOWS_BFW

#include "LinkFlowsCFW.h"

/** \brief This class implements bi-conjugate Frank-Wolfe algorithm.
	\details For details see \cite Mitradjieva2012.
	\note Calculation of the coefficients is a result of experimentation to make the algorithm
	converge. It is not explicitly mentioned in the paper how it should be done. 
	\warning This algorithm shows problems with convergence when precision is high.
*/
class LinkFlowsBFW : public LinkFlowsCFW {

	public:
		LinkFlowsBFW(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, LineSearch* lineSearch, FPType zeroFlow,
				LabelCorrectingAl* LCShPath);
		virtual ~LinkFlowsBFW();
		
	private:
		FPType *linkFlowsBFW_; /**< BFW link flows */
		
		/** Calculates BFW coefficients.
		*/
		void calcCoeff(FPType &beta0, FPType &beta1, FPType &beta2);
		virtual void calculateDirection();

};

#endif
