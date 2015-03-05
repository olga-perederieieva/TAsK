#ifndef LINEAR_INDIFF_CURVE
#define LINEAR_INDIFF_CURVE 

#include "IndiffCurve.h"

/** \brief Implemenats linear scalarization function.
*/
class LinearIndiffCurve : public IndiffCurve {
	public:
		/** \note toll1 is always zero.
			Function is \f$f(toll) = a * toll + time1\f$ where
			\f$a = (time2 - time1) / toll2\f$.
		**/
		LinearIndiffCurve(FPType time1, FPType time2, TollType toll2);
		/** If coefficient a and b are already known, then
			function is \f$f(toll) = a * toll + b\f$.
		*/
		LinearIndiffCurve(FPType a, FPType b);
		~LinearIndiffCurve();

		virtual FPType evaluate(TollType toll);

		virtual std::string toString();

	private:
		FPType a_;
		FPType b_;
};

#endif