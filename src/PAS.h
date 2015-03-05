#ifndef PAS_
#define PAS_

#include "UsedTypes.h"

#include <list>
#include <set>

class DAGraphTapas;
class StarLink;

/** \brief This class represents pair of alternative segments (PAS).
	\details For details see \cite Hillel2010.
	\note The definition of what PAS is unused is different from the paper, see isUnused().
*/
class PAS {
	public:
		explicit PAS(FPType zeroFlow, FPType dirTol);
		virtual ~PAS();
		
		/** @return true if PAS is unused, false otherwise. 
			PAS is unused if the flow was not moved or if the entire origin flow from expensive segment 
			was moved.
			\note This definition of unused PAS is different from what is written in the paper
			(according to numerical experiments it works faster - it is better to delete PAS more often).
		*/
		bool isUnused();
		
		/** This method is used for creation of segments of current PAS. Adds link to the front of
			cheap cost segment.
		*/
		void pushFrontToCheap(StarLink *link);
		/** This method is used for creation of segments of current PAS. Adds link to the front of
			expensive cost segment.
		*/
		void pushFrontToExp(StarLink *link);
		/** This method is used for creation of segments of current PAS. Adds link to the back of
			cheap cost segment.
		*/
		void pushBackToCheap(StarLink *link);
		/** This method is used for creation of segments of current PAS. Adds link to the back of
			expensive cost segment.
		*/
		void pushBackToExp(StarLink *link);
		
		/** @return last link in the cheap segment. NULL is returned if the segment is empty.
			This method is used in PASManager for checking if PAS exists already.
		*/
		StarLink* getLastCheapLink();
		/** @return last link in the expensive segment. NULL is returned if the segment is empty.
			This method is used in PASManager for checking if PAS exists already.
		*/
		StarLink* getLastExpLink();
		

		/** Adds bush \b dag to the set of relevant origins of this PAS.
			If this bush is already in the set,	it won't be added.
			@return true if origin wad added, false otherwise.
			\note The pointer to entire bush is stored instead of origin index in order to
			facilitate access to some information required by PAS.
		*/
		bool addOrigin(DAGraphTapas *dag);
		
		/** Performs flow move within PAS, updates corresponding link flows and travel times.
			@return true if flow was actually moved, false otherwise.
		*/
		bool moveFlow();

		/** Recalculates costs of cheap and expensive segments and updates corresponding pointer 
			to cheap segment (after several iterations cheap segment might become an expensive one).
			@return cost difference of segments
		*/
		FPType recalcPASCosts();
		
		/** Checks if current PAS has flow and is cost effective (for details see \cite Hillel2010).
			@param cost reduced cost.
			@param v a specific parameter of TAPAS that is used in order to consider a PAS as flow effective.
			@param index index of the expensive link of bush under investigation.
			@param dag pointer to a bush under investigation.  
			@return true if PAS is effective, false otherwise.
		*/
		bool checkIfEffective(FPType cost, FPType v, int index, DAGraphTapas* dag);
		
		/** For debugging. Prints segments on screen. */
		void print() const;
		/** @return total number of links belonging to both segments. */
		int getNbLinks() const;
		/** @return total number of origin associated with this PAS.
		*/
		int getNbOrigins() const;
		/** @return cost difference between two segments.
		*/
		FPType getCostDiff();

	protected:

		std::list<StarLink*> segments_[2];
		FPType cheapCost_;
		FPType expCost_;
		FPType totalShift_; /**< minimum possible total shift of flow */
		int cheapSegm_;

		/** Calculates flow shift that must be performed from expensive segment to cheap one. 
		*/
		virtual FPType getFlowShift();
		
	private:
		
		typedef std::set<DAGraphTapas*> SetType;
		SetType relevantOrigins_; /**< set is used in order to have log n complexity on insertion. */
									/**< Also uniqueness is ensured. */
		static FPType zeroFlow_;
		static FPType dirTol_;
		
		int nbFlowMoves_;
		
		/** Calculates minimum possible shifts for each origin and minimum possible total shift.
			It also calls getFlowShift() and ensures that it does not exceed minimum possible
			total shift.
		*/
		FPType calculateFlowShift();

		/** Calculates cost of a segment given its index.
		*/
		FPType calcSegCost(int index);
		
		/** Checks if current PAS is cost effective. 
			@param cost reduced cost.
			@return true if PAS is cost effective, false otherwise.
		*/
		inline bool checkIfCostEffective(FPType cost) const;
		/** Checks if current PAS is flow effective.
			@param v  a specific parameter of TAPAS.
			@param index index of the expensive link of bush under investigation.
			@param dag pointer to a bush under investigation.
			@return true if PAS is flow effective, false otherwise.
		*/
		bool checkIfFlowEffective(FPType v, int index, DAGraphTapas* dag) const;
		
};
#endif
