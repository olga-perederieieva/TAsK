#ifndef CONST_FNC
#define CONST_FNC

#include "LinkFnc.h"

/** \brief This class implements constant link cost function.
	\details The equation is as follows: \f$ c_a(f_a) = param\f$.
	*/
class ConstFnc : public LinkFnc {
	public:
		ConstFnc(FPType param);
		~ConstFnc();
		
		FPType evaluate(FPType linkFlow) const;
		FPType evaluateDerivative(FPType linkFlow) const;
		void print() const;
		virtual FPType evaluateTimeAndDerivative(FPType linkFlow, FPType &der);
		
		virtual FncType getType();

		virtual std::string toString() const {
			return "constant fnc";
		};
		
		LinkFnc* clone() const {return new ConstFnc(*this);}
	private:
		const FPType param_;
};

#endif
