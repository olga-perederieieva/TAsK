#ifndef PAS_SET
#define PAS_SET

#include "UsedTypes.h"

#include <vector>
#include <list>
#include <tr1/unordered_set>

class ShortestPath;
class DAGraphTapas;
class PAS;
class StarLink;

typedef std::list<PAS*>::const_iterator CostPasIterator;

/** \brief This class is a container of PASs and it implements major operations related to
	their management.
*/
class PASManager {
	public:
		/** @param shPath shortest path algorithm.
			@param dirTol tolerance of the direction of descent.
			@param nbNodes number of nodes in the network.
			@param mu TAPAS-specific constant, see \cite Hillel2010.
			@param v TAPAS-specific constant, see \cite Hillel2010.
			@param zeroFlow link flow tolerance.
		*/
		explicit PASManager(const ShortestPath &shPath, FPType dirTol, int nbNodes,
							FPType mu, FPType v, FPType zeroFlow);
		virtual ~PASManager();
		
		/** Checks if PAS with a specified expensive link \b expLink already exists.
			If it exists, method adds current bush to the set of relevant origins of the 
			existing PAS and checks if current PAS is effective.
			If PAS does not exists the method creates a new PAS (if it is possible) and
			checks if it is effective.
			If created or existing PAS is not effective, one more PAS is created that is effective.
			\note Branch shift is not implemented.
		*/
		void createNewPAS(DAGraphTapas *dag, StarLink *expLink, int mergingNodeIndex); 

		/** Loops through all PASs and checks if they are unused. If a PAS is unused, 
			it is deleted from the PAS set. Otherwise, flow is moved within that PAS.
		*/
		void deleteUnusedPASAndMoveFlow();
		
		/** @return iterator that points to the beginning of PAS container. 
		*/
		CostPasIterator begin() const;
		/** @return end of iterator.
		*/
		CostPasIterator end() const;
		/** @return total number of PAS.
		*/
		int getNbPAS() const;
		/** Prints all PAS on screen.
		*/
		void print() const;
		/** Loops through all PAS and recalculates their path costs.
		*/
		void recalculatePASCosts();
		
	protected:
		
		const FPType zeroFlow_;
		const FPType dirTol_;

		/** Allocates PAS of an appropriate type and returns pointer to it.
		*/
		virtual PAS* allocatePAS();

	private:
		const ShortestPath &shPath_; 
		std::list<PAS*> listOfPas_; 
		
		const int nbNodes_;
		const FPType mu_;
		const FPType v_;
		
		std::tr1::unordered_set<int> setOfNodes_; /**< At the moment tr1 unordered set is used, 
													alternative is std set. 
												\note tests on bigger instances are 
												needed to see the difference in performance
												of different data structures.
												At the moment it does not make a difference. */

		int nbIter_;
		
		/** Checks if PAS whose segments end with merging links exists. 
			@returns pointer to found PAS, NULL otherwise.
		*/
		PAS* pasExist(StarLink* cheapLink, StarLink* expLink);
		
		/** Checks if node with index \b nodeIndex belongs to current shortest path 
			of the current shortest path tree.
		*/
		bool nodeBelongsToShPath(int nodeIndex);
		
		/** Backtracks expensive segment from BFS-tree.
		*/
		void createExpSegment(PAS *pas, const std::vector<StarLink*> &checkedLinks, int divergeNode, 
								int mergingNodeIndex);
		/** Builds cheap segment by starting from merging index \b mergingNodeIndex and 
			stops when \b divergeNode or origin is reached.
		*/
		void createCheapSegment(PAS *pas, int divergeNode, int mergingNodeIndex);

		/** Fills setOfNodes_ that ensures fast access to shortest path.
		*/
		inline void prepareShPathStructure(int mergingNodeIndex);
		
		/** @return calculated reduced cost.
		*/
		FPType calculateReducedCost(StarLink *expLink);
		
		/** @return threshold depending on current iteration. It is required for effectiveness check.
		*/
		FPType calcThreshold();

		/** Creates PAS (including effective PAS): creates cheap and expensive segments if is possible
			and adds it to internal PAS container.
		*/
		PAS* createPAS(DAGraphTapas *dag, StarLink *expLink, int mergingNodeIndex, 
					bool (*checkPtr)(FPType originFlow, FPType thr));
		
		
};

#endif
