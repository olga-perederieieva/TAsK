#ifndef _ORIGIN_BASED_ALGO_
#define _ORIGIN_BASED_ALGO_

#include "EqAlgo.h"

class OriginSet;
class StarNetwork;
class OriginBush;

/** \brief This class implements main steps of the framework for origin-based algorithms.
*/
class OriginBasedAlgo : public EqAlgo {
	public:
		/** @param originSet pointer to object that stores all bushes.
			@param net network.
			@param component pointer to object that performs extra operation with convergence data.
			@param conv convergence measure.
			@param timeLimit maximum allowed computational time.
		*/
		OriginBasedAlgo(OriginSet *originSet, StarNetwork *net, AddHook *component, 
						ConvMeasure* conv, FPType timeLimit);
		virtual ~OriginBasedAlgo();
		
	protected:
		/** This is a hook method that does nothing by default. It is executed after each iteration.
			It is re-implemented in OriginBasedAlgoTapas.
		*/
		virtual void doSmthAfterOrigins(){}; 

		/** Performs main operations made during one iteration for a given origin.
			It implements equilibration I. Equilibration II over-rides this method.
			For details see OriginBasedAlgoEQII.
			@return true if current bush was equilibrated, false otherwise. The return value is needed
			only for equilibration II.
		*/
		virtual bool mainLoop(OriginBush *bush); 
		
	private:
		OriginSet *originSet_;

		virtual void initialise();
		virtual void performOneIteration();
};

#endif
