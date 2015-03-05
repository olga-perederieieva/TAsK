#ifndef ALGO_DECORATOR_BASE
#define ALGO_DECORATOR_BASE

#include "DecoratedEqAlgo.h"

/** \brief Base class for algorithm decorators. Defines common methods for derived classes.
*/
class AlgoDecoratorBase : public DecoratedEqAlgo { 
	public:
		
		virtual ~AlgoDecoratorBase() {delete component_;};
		virtual int execute() = 0;

		/** Method inherited from DecoratedEqAlgo. Redirects to component_.*/
		virtual FPType getTotalConvTime() {return component_->getTotalConvTime();};
		/** Method inherited from DecoratedEqAlgo. Redirects to component_.*/
		virtual FPType getTotalPureIterTime() {return component_->getTotalPureIterTime();};
		/** Method inherited from DecoratedEqAlgo. Redirects to component_.*/
		virtual FPType getInitialisationTime() {return component_->getInitialisationTime();};

	protected:

		DecoratedEqAlgo* component_; /**< algorithm for decoration, it 
										is deallocated in this class.*/
		/** @param component algorithm for decoration.*/
		AlgoDecoratorBase(DecoratedEqAlgo* component): component_(component) {};

};
#endif