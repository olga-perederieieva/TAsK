#ifndef ORIGIN_BUSH_TAPAS
#define ORIGIN_BUSH_TAPAS

#include "OriginBush.h"

class DAGraphTapas;
class PASManager;
class ShortestPath;

/** \brief This class is responsible for DAGraphTapas creation. 
	It also over-rides some methods of OriginBush
	that are different for TAPAS algorithm.
*/
class OriginBushTapas : public OriginBush {
	public:
		OriginBushTapas(ShortestPath *shPath, PASManager *pasManager);
		~OriginBushTapas();
		
		/** Removes cyclic flows.
		*/
		virtual void updateTopSort();
		
		/** Does nothing and always returns false (Equilibration II for TAPAS is 
			implemented in OriginBasedAlgoTapasEqII).
		*/
		virtual bool improve() {return false;};
		
		/** Does nothing.*/
		virtual void removeUnusedLinks() {};
	
	private:
		DAGraphTapas *dag_;
		static ShortestPath *shPath_;
		static PASManager *pasManager_;

		virtual DAGraph* createNewDAG(int index, StarNetwork *net, ODMatrix *mat,
									  FPType zeroFlow, FPType dirTol);
};

#endif
