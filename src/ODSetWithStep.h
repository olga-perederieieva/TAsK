#ifndef OD_SET_WITH_STEP
#define OD_SET_WITH_STEP

#include "ODSet.h"
#include "DescDirection.h"

class PathBasedFlowMoveWithStep;

/** \brief This class implements strategy of moving flow when step size is calculated.
	\details It is used in approaches 1 and 2 of path-based algorithms.
*/
class ODSetWithStep : public ODSet, public DescDirection {
	public:
		ODSetWithStep(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net,
				ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMoveWithStep* flowMove);
		~ODSetWithStep();
		
		// from DescDirection
		FPType* getProjectedDirection();
		FPType getUpperBound();
		FPType* getLinkFlows();
		int getSize();
		int* getIndexes();
		virtual int getOdIndex() const;
		virtual PathBasedFlowMove* getPathBasedFlowMove() const;
		
	private:
		static FPType *projDir_; /**< projected direction of descent */
		static int *indexes_;
		static int size_;
		static PathBasedFlowMoveWithStep* flowMoveStep_;
		static FPType *linkFlows_;
		static bool wasInitialised_;
		static int nbLinks_;
		
		void calculateProjectedDirection();
};

#endif
