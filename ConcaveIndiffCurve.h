#ifndef CONCAVE_INDIFF_CURVE
#define CONCAVE_INDIFF_CURVE 

#include "IndiffCurve.h"

/** \brief Implements convex scalarization function.
*/
class ConcaveIndiffCurve : public IndiffCurve {
	public:
		/** \note toll1 is always zero.
			Function is \f$f(toll) = a * toll^{1/3} + time1\f$ where
			\f$a = (time2 - time1) / toll2^{1/3}\f$.
		*/
		ConcaveIndiffCurve(FPType time1, FPType time2, TollType toll2);
		/** If coefficient a and b are already known, then
			function is \f$f(toll) = a * toll^{1/3} + b\f$.
		*/
		ConcaveIndiffCurve(FPType a, FPType b);
		~ConcaveIndiffCurve();

		virtual FPType evaluate(TollType toll);

		virtual std::string toString();

	private:
		FPType a_;
		FPType b_;

};
#endif