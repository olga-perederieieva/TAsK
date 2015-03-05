#ifndef LINEAR_FNC
#define LINEAR_FNC

#include "LinkFnc.h"

/** \brief This class implements linear link cost function.
	\details t(x) = a*x + b.
 */
class LinearFnc : public LinkFnc {
	
	public:
	
		LinearFnc(FPType a, FPType b);
		~LinearFnc();
	
		FPType evaluate(FPType arcFlow) const;
		FPType evaluateDerivative(FPType arcFlow) const;
		virtual FPType evaluateTimeAndDerivative(FPType linkFlow, FPType &der);

		void print() const;

		virtual std::string toString() const {
			return "linear fnc";
		};

		virtual FncType getType();
	
	private:
	
		const FPType _param1;
		const FPType _param2;
};

#endif
