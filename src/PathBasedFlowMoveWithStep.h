#ifndef PATH_BASED_FLOW_MOVE_WITH_STEP
#define PATH_BASED_FLOW_MOVE_WITH_STEP

#include "PathBasedFlowMove.h"
#include "UsedTypes.h"

class StepSize;
class DescDirection;

/** \brief This class implements path-based flow move of an equilibration algorithm 
	(it depends on DescDirectionPath class) 
	with line search for step size calculation (approaches 1 and 2).
*/
class PathBasedFlowMoveWithStep : public PathBasedFlowMove {
	public:
		/** @param stepSize object that implements step size calculation.
			@param pathDirectionAlgo path-based direction of descent.
			@param zeroFlow link flow tolerance .
		*/
		PathBasedFlowMoveWithStep(StepSize *stepSize, DescDirectionPath* pathDirectionAlgo,
								  FPType zeroFlow);
		~PathBasedFlowMoveWithStep();
		
		/** @return true if calculated step size is zero, false otherwise.
		*/
		bool executeFlowMove();
		
		/** Sets the current direction of descent. 
			\note It must be executed each time a new O-D pair is considered.
		*/
		void setDescDirection(DescDirection* dir);
		
	private:
		StepSize *stepSize_;
		const FPType zeroFlow_;
		DescDirection* dir_;
};

#endif
