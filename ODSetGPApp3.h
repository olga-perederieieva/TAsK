#ifndef OD_SET_GP_APP3
#define OD_SET_GP_APP3

#include "ODSet.h"

class PathBasedFlowMoveGP;
class ODMatrix;

/** \brief This class extends ODSet for gradient projection as described in
	the paper of  \cite Jayakrishnan1994.
	\details For details see \cite Jayakrishnan1994. 
*/
class ODSetGPApp3 : public ODSet{
	public:
		ODSetGPApp3(int index, int destIndex, int originIndex, PathCost *pathCost, StarNetwork *net,
				ShortestPath *shPath, FPType zeroFlow, PathBasedFlowMoveGP* flowMove, ODMatrix *mat);
		~ODSetGPApp3();
		
	private:
		static ODMatrix* mat_;
		static PathBasedFlowMoveGP* flowMoveGP_;
		
		/** Sets demand value of current O-D pair that is required by PathBasedFlowMoveGP.
		*/
		virtual void calculateProjectedDirection();
		
};

#endif
