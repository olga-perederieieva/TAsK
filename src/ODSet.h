#ifndef OD_SET
#define OD_SET

#include "UsedTypes.h"

#include <stdlib.h>
#include <list>

class PathCost;
class Path;
class StarNetwork;
class ShortestPath;
class PathBasedFlowMove;

typedef std::list<Path*>::const_iterator PathIterator;

/** \brief This class represents the O-D set of paths. It is a basic element of PathSet class.
*/
class ODSet { 
	public:
		/** @param index O-D pair index.
			@param destIndex destination index.
			@param originIndex origin index.
			@param pathCost object that knows how to calculate path cost.
			@param net network.
			@param shPath shortest path algorithm.
			@param zeroFlow link flow tolerance.
			@param flowMove algorithm for calculating flow move.
		*/
		ODSet(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net, 
			ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMove* flowMove);
		virtual ~ODSet();

		/** @return current minimum path cost.
		*/
		FPType getMinPathCost() const;
		
		/** @return O-D pair index. */
		int getIndex() const;
		/** @return destination node index.*/
		int getDestIndex() const;
		/** @return origin node index.*/
		int getOriginIndex() const;
		
		/** @return number of paths stored in this O-D set.*/
		int getNbPaths() const;
		
		/** Adds path to the set of paths.
		*/
		void addPath(Path *path);
		
		/** @return iterator to the beginning of the container with paths.*/
		PathIterator begin() const;
		
		/** @return end of container.*/
		PathIterator end() const;
		
		/** Goes through all paths stored in this O-D set and recalculates their costs.
			It also updates current minimum path cost.
		*/
		void updateSet();

		/** Stored path flow before they change.
		*/
		void setPrevFlows();
		
		/** Calculates shortest path for current O-D pair and checks 
			if it is shorter than the current shortest path in the set. If it is shorter, then
			it is added to the set.
			@return true if path was added to the set, false otherwise.
		*/
		bool improveSet();
		
		/** Calculates direction of descent in terms of path flows and moves flow 
			according to some strategy that depends on the algorithm.
			For algorithms see PathBasedFlowMove class.
		*/
		bool equilibrate();
		
		/** Projects path flows on links and updates their costs.
		*/
		void projectPathFlowOnLinks();
		
		/** Removes unused paths from the set. The path is unused if its flow is less than ZERO_FLOW.
		*/
		void removeUnusedPaths();
		
		void print() const;
		
	protected:
		
		/** Does nothing by default - it is a hook method for ODSetWithStep. 
		*/
		virtual void calculateProjectedDirection() {}; 

		const int destIndex_;
		const int originIndex_;
			
	private:
		static StarNetwork *net_;
		static ShortestPath *shPath_;
		
		const int index_; 
		
		static PathCost *pathCost_;
		FPType minDist_;
		static FPType zeroFlow_;
		
		std::list<Path*> allPaths_;
		
		static PathBasedFlowMove *flowMove_;
		
};

#endif
