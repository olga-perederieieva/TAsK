#ifndef DAGRAPH_B
#define DAGRAPH_B

#include "DAGraph.h"

class Path;

/** \brief This class implements origin-based algorithm B.
	\details For details see \cite Dial_2006.
*/
class DAGraphB : public DAGraph {
	public: 
		
		/** 
			@param useMultiStep true if multiple Newton steps must be applied, false otherwise.
			@param dirTol tolerance of the direction of descent.
		*/
		DAGraphB(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
				 int originIndex, bool useMultiStep);
		~DAGraphB();
		
		bool moveFlow();
		
	protected:
		/** Implements Newton flow step. See DAGraphBWithStep for other 
			step size implementations.
		*/
		virtual FPType calcFlowStep(Path* minPath, Path* maxPath) const;

		int nbIter;
	private:
		
		static bool useMultiStep_;
		static FPType dirTol_;

		
		/** Perform a flow move: flow is moved from longest path to shortest path 
		 	that belong to the bush and start at \b origin and finish at node \b index.
		 	It is assumed that \b minPath 
			and \b maxPath do not have common links.
		*/
		bool performFlowMove(int index, int origin);
		
		/** Adds flow \b dFlow to \b link, updates corresponding origin flow and
			recalculates travel time of \b link.
		*/
		void addFlow(StarLink *link, FPType dFlow);

		/** For debugging. */
		void printMinPathOFlows(int index) const;
		/** For debugging. */
		void printMaxPathOFlows(int index) const;
		
};

#endif
