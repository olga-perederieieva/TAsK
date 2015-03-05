
#ifndef EQ_ALGO
#define EQ_ALGO
#include "DecoratedEqAlgo.h"
#include "UsedTypes.h"

class AddHook;
class StarNetwork;
class ConvMeasure;

/** \brief This class is a base class for all traffic assignment algorithms.
*/
class EqAlgo : public DecoratedEqAlgo {
	
	public: 
		virtual ~EqAlgo();
		
		/** @return total convergence time.
		*/
		virtual FPType getTotalConvTime();

		/** @return total pure iteration time.
		*/
		virtual FPType getTotalPureIterTime();

		/** @return initialisation time needed to perform all-or-nothing assignment.
		*/
		virtual FPType getInitialisationTime() ;

		/** Executes the algorithm.
			@return total number of iterations needed to converge to a given level of precision.
		*/
		int execute();
		
	protected:
		ConvMeasure* conv_;

		/**	@param component object that performs extra operations with convergence information,
				see AddHook.
			@param net network
			@param timeLimit time limit in seconds when algorithm must be stopped before reaching
				required level of precision.
			@param conv convergence measure.
		*/
		EqAlgo(AddHook *component, StarNetwork* net, FPType timeLimit, 
				ConvMeasure* conv);

		/** Initialises solution structure of the algorithm. Usually it performs AON assignment.
		*/		
		virtual void initialise() = 0;

		/** Performs one iteration of the algorithm.
		*/
		virtual void performOneIteration() = 0;

		/** Convergence check might be over-ridden by children classes. For example, see FWAlgo. 
		*/
		virtual bool isConverged();

		StarNetwork *net_;
	private:	
		AddHook *component_;
		
		const FPType timeLimit_;
		FPType totalConvTime_;
		FPType totalPureIterTime_;
		FPType initialisationTime_;
};

#endif
