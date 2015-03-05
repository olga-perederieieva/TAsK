#ifndef DAGRAPH_TAPAS
#define DAGRAPH_TAPAS

#include "DAGraph.h"

class ShortestPath;
class PASManager;

/** \brief Implements main steps of TAPAS algorithm.
	\details For details see \cite{Hillel2010}.
	\note This implementation of TAPAS is not randomized,
	it does not implement flow proportionality condition.
	Also different condition is used in order to determine
	if PAS is inactive (PAS is inactive if flow was not 
	moved during previous iteration). 
	The following threshold is used in case of effective PAS creation \f$ 10 \cdot 10^{-iter}\f$ where
	\f$ iter \f$ is current iteration number. Performance of the algorithm depends on this measure.
	Only creation of effective PASs is implemented. Brunch shift is not implemented. 
*/
class DAGraphTapas : public DAGraph {
	public:
  		explicit DAGraphTapas(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol,
  							  int originIndex, ShortestPath *shPath, PASManager *pasManager);
		~DAGraphTapas();
		
		/** Implements main steps of TAPAS including creation of PASs and flow
			moves within ALL active PASs.
		*/
		bool moveFlow();

		/** Stores the value of minimum shift. It is calculated and used in PAS.*/
		void setMinShift(FPType value);

		/** @return value of possible minimum shift. */
		FPType getMinShift();
		
		/** This method removes cyclic flows. */
		void removeCyclicFlows();		
		
	private:
		static ShortestPath *shPath_;
		static PASManager *pasManager_;
		FPType minShift_;

		
		std::list<StarLink*> exploredLinks_; /**< Stores explored links 
								during topological sort. It is used in handleBackEdge().*/
		
		/** This method is called in topologicalSort(). When a back edge is discovered,
		 	it removes cyclic flows
			and updates corresponding link travel times.
			@return always returns true. It is used in order to stop recursive calls
			in topologicalSort().
		*/
		bool handleBackEdge(StarLink* link);

		/** This method stores explored link into internal data structure.*/
		void handleExploredLink(StarLink* link);

		/** Additional condition in topologicalSort(). In case of TAPAS topologicalSort()
			must check only links used by this origin, i.e. links with positive origin flow.
			@return true if origin flow of passed link is greater than zeroFlow_, false otherwise.
		*/
		bool checkPositiveFlow(int linkIndex);
		
};

#endif
