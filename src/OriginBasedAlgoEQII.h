#ifndef ORIGIN_BASED_ALGO_EQII
#define ORIGIN_BASED_ALGO_EQII

#include "OriginBasedAlgo.h"

/** \brief This class implements equilibration II strategy for origin-based algorithms.
	\warning Equilibration II might not converge in case of high precision.
*/
class OriginBasedAlgoEQII : public OriginBasedAlgo {
	public:
		/** @param maxNbIter maximum number of iteration in Equilibration II.
		*/
		OriginBasedAlgoEQII(OriginSet *originSet, StarNetwork *net, AddHook *component, 
							ConvMeasure* conv, FPType timeLimit, int maxNbIter);
		~OriginBasedAlgoEQII();
	
	private:	
		const int maxNbIter_;

		bool mainLoop(OriginBush *bush);
};

#endif
