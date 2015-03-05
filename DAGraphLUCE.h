#ifndef DAGRAPH_LUCE
#define DAGRAPH_LUCE

#include "DAGraph.h"
#include "LineSearch.h"

#include <vector>

/** \brief Implements origin-based algorithm LUCE.
	\details For details see \cite Gentile2014. 
	\note Current implementation is a bit different from what is said in the paper.
	It is origin based instead of destination based.
	In many places algorithm iterates through bushes instead of network.
	Also link travel times are updated on the fly only when the flow there changes.
	\warning This algorithm showed some problems with converge when required precision is high. 
	The possible reason is problems with floating point operations, big amounts of flow might be lost
	after several iterations.
	Another problematic issue with this algorithm is related to degenerate situation when derivative is
	zero. At the moment this problem is solved by setting the derivative to one.
*/
class DAGraphLUCE : public DAGraph {
	public:

		DAGraphLUCE(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
					int originIndex, LineSearch* lineSearch);
		~DAGraphLUCE();
		
		/** Implements flow move of LUCE.
			@return always returns false because this algorithm is not meant to
			implement Equilibration II.
		*/
		virtual bool moveFlow();
		
		/** Sets derivative to zero first and then recalculates it for the links that belong 
			to the bush. It also calculates flow portions and node flows. 
			It is required that topological order exists.
		*/
		void prepareData(); 
		
	private:
		
		const int nbNodes_;
		const int nbLinks_;
		std::vector<FPType> nodeFlowsInv_;
		std::vector<FPType> flowPortionsInv_;
		
		static LineSearch* lineSearch_;

		void calcFlowPortions();
		
		void computeAvCosts(std::vector<FPType> &C, std::vector<FPType> &G);
		void computeDirection(std::vector<FPType> &e_links, const std::vector<FPType> &C,
							  const std::vector<FPType> &G);
		FPType computeStepSize(const std::vector<FPType> &e_links);
		void assignNewFlow(FPType stepSize, const std::vector<FPType> &e_links);

};

#endif
