#ifndef LINK_FLOWS_
#define LINK_FLOWS_

#include "InitialiseSolution.h"
#include "AONUsual.h"

class LineSearch;
class ODMatrix;
class LabelCorrectingAl;
class StarNetwork;
class ShortestPath;

/** \brief This class implements basic operations needed for link-based algorithms
	and Frank-Wolfe algorithm.
	\details For details see \cite Sheffi1985. 
	\warning This algorithm shows problems with convergence when precision is high.
*/
class LinkFlows : public InitialiseSolution { 
	public:

		LinkFlows(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath,
					LineSearch* lineSearch, FPType zeroFlow, 
					LabelCorrectingAl* LCShPath);
		virtual ~LinkFlows();
		
		/** Calculates link costs, calls AON assignment and initialises link flows (stored in
			array) with AON flows.
		*/
		void initialise();
		
		/** Calculates direction of descent and step size. Then it calculates new link flows by modifying 
			internal data structure.
		*/
		void equilibrate();
		
		/** Loads link flows from internal data structure to StarNetwork object and updates link costs.
		*/
		void updateLinkFlows();

		/** @return minimum travel time calculated in AON assignment.
		*/
		FPType getMinTravelTime() const;
		
		/** Initialises link flows by adding the demand of destination \b dest to existing link flow.
		*/
	 	void initialiseItself(StarLink* link, PairOD *dest, int originIndex);
	 	
	protected:
		
		const int nbLinks_;
		FPType *linkFlows_; /**< current link flows */
		FPType *linkFlowsAux_; /**< current AON flows */
		
		FPType stepPrev_; /**< current step size obtained from line search */
		FPType stepPrevPrev_; /**< previous step size, it is needed in BFW */

		const FPType zeroFlow_;

		LineSearch* lineSearch_; /**< LineSearch pointer is protected because CFW and BFW require */
									/**< access to link cost functions in order to evaluate derivative*/

		FPType *direction_; /**< current direction of descent \note it must be
								modified directly by children classes */

		StarNetwork *net_;
		
		/** Calculates AON flows and stores them into linkFlowsAux_.
		*/
		void calculateFWAux();

		/** Calculates the direction of descent.
		*/
		virtual void calculateDirection();

	private:
		AONUsual aon_;
		
		int *indexes_;
		FPType minTravelTime_;
		
};

#endif
