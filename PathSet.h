#ifndef PATH_SET
#define PATH_SET

#include "InitialiseSolution.h"

#include <vector>

class ODSet;
class ODMatrix;
class PathCost;
class PathBasedFlowMove;
class PathBasedFlowMoveWithStep;
class PathBasedFlowMoveGP;
class AONAssignment;
class Path;
class ShortestPath;
class StarNetwork;

typedef std::vector<ODSet*>::const_iterator ConstODSetIterator;
/** \brief This class represents a container for ODSet objects.
*/
class PathSet : public InitialiseSolution {
	public:

		/** This method must be called for algorithms that implement approaches 1 or 2. 
		*/
		static PathSet* createSetWithStep(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath,
			PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMoveWithStep* flowMove, AONAssignment* aon);

		/** This method must be called for path equilibration algorithm approach 3. 
		*/		
		static PathSet* createSetPEAPP3(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMove* flowMove, AONAssignment* aon);
		
		/** This method must be called for gradient projection algorithm approach 3. 
		*/
		static PathSet* createSetGPAPP3(StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMoveGP* flowMove, AONAssignment* aon);
		~PathSet();
		
		void initialise();
		
		void initialiseItself(StarLink* link, PairOD *dest, int originIndex);
		
		/** @return iterator pointing to the first O-D set.
		*/
		ConstODSetIterator begin() const;
		/** @return end of container.
		*/
		ConstODSetIterator end() const;

		/** @return pointer to O-D set given O-D pair index odIndex.
		*/
		ODSet* getODSetByIndex(int odIndex) const;
		/** Prints path sets on screen.
		*/
		void print();
		
	private:
		std::vector<ODSet*> odSetList_;
		const int size_;
		Path *currPath_;
		
		AONAssignment* aon_;

		PathSet(ODSetType setType, StarNetwork *net, ODMatrix *mat, ShortestPath *shPath, PathCost *pathCost, 
			FPType zeroFlow, PathBasedFlowMove* flowMove,  
			PathBasedFlowMoveWithStep* flowMoveWithStep, PathBasedFlowMoveGP* flowMoveGP,
			AONAssignment* aon);
};

#endif
