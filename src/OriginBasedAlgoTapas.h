#ifndef ORIGIN_BASED_ALGO_TAPAS
#define ORIGIN_BASED_ALGO_TAPAS

#include "OriginBasedAlgo.h"
#include "PASManager.h"

/** \brief This class extends OriginBasedAlgo by adding functionality required by TAPAS.
*/
class OriginBasedAlgoTapas : public OriginBasedAlgo {
	public:
		explicit OriginBasedAlgoTapas(OriginSet *originSet, StarNetwork *net, PASManager* pasSet, 
				AddHook *component, ConvMeasure* conv, FPType timeLimit);
		~OriginBasedAlgoTapas();
	
	private:
		PASManager* pasSet_;
		
		/** Removes unused PASs from PASManager and moves flows within PASs that
			were not deleted.
		*/
		void doSmthAfterOrigins();	
};

#endif
