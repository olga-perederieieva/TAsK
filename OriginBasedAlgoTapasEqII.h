#ifndef ORIGIN_BASED_ALGO_TAPAS_EQII
#define ORIGIN_BASED_ALGO_TAPAS_EQII

#include "OriginBasedAlgoTapas.h"

/** \brief Implements Equilibration II for TAPAS.
*/
class OriginBasedAlgoTapasEqII : public OriginBasedAlgoTapas {
public:
	
	OriginBasedAlgoTapasEqII(OriginSet *originSet, StarNetwork *net, PASManager* pasSet, 
				AddHook *component, ConvMeasure* conv, FPType timeLimit, 
				int maxNbIter);
	
	~OriginBasedAlgoTapasEqII();

private:	
		const int maxNbIter_;

		bool mainLoop(OriginBush *bush);
};

#endif