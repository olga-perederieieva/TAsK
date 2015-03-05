#ifndef LINK_FCN
#define LINK_FCN

#include "UsedTypes.h"
#include "Constants.h"

#include <string>

/** \brief Abstract class used to represent link cost function. 
	\details Any concrete link cost function class MUST inherit this
	class and implement virtual methods.
 */
class LinkFnc {
	
	public:
	
		virtual ~LinkFnc(){};
		
		/** Evaluates link cost function for a given amount of flow. */
		virtual FPType evaluate(FPType linkFlow) const =0;

		/** Evaluates derivative of link cost function for a given amount of flow. */
		virtual FPType evaluateDerivative(FPType linkFlow) const =0;

		/** Evaluates second derivative of link cost function for a given amount of flow. */
		virtual FPType evaluateSecondDerivative(FPType linkFlow) const { return 0.0; };
		
		/** Prints link cost function parameters on screen.
		*/
		virtual void print() const =0;

		/** @return string describing link cost function parameters.
		*/
		virtual std::string toString() const =0;
		
		/** Creates a copy of the link cost function.
		*/
		virtual LinkFnc * clone() const =0;

		/** Evaluates both function and its derivative.
			\note Derivative is written in the input parameter der.
			@return evaluated function value.
		*/
		virtual FPType evaluateTimeAndDerivative(FPType linkFlow, FPType &der){
			der = evaluateDerivative(linkFlow);
			return evaluate(linkFlow);
		};

		/** @return 0.0 if not overridden.
		*/
		virtual FPType getCapacity() {
			return 0.0;
		};

		/** This method is implemented only in SpiessFnc, SpiessFncComb, PlusLinearFnc.
			@return 0.0 if not overridden.
		*/
		virtual FPType getLength() const {
			return 0.0;
		};

		/** This method is implemented only in SpiessFnc, SpiessFncComb, PlusLinearFnc.
			Does nothing if not overridden.
		*/
		virtual void setLength(FPType length) {

		};

		/** @return type of the function. All types are listed in Constants.h.
		*/
		virtual FncType getType() = 0;
	
	protected:
		LinkFnc(){};
};

#endif
