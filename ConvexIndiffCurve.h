#ifndef CONVEX_INDIFF_CURVE
#define CONVEX_INDIFF_CURVE 

#include "IndiffCurve.h"

/** \brief Implemenats convex scalarization function.
*/
class ConvexIndiffCurve : public IndiffCurve {
	public:

		/** \note toll1 is always zero.
			Function is \f$f(toll) = a * toll^{3} + time1\f$ where
			\f$a = (time2 - time1) / toll2^{3}\f$.
		**/
		ConvexIndiffCurve(FPType time1, FPType time2, TollType toll2);
		/** If coefficient a and b are already known, then
			function is \f$f(toll) = a * toll^{3} + b\f$.
		*/
		ConvexIndiffCurve(FPType a, FPType b);
		~ConvexIndiffCurve();

		virtual FPType evaluate(TollType toll);

		virtual std::string toString();

	private:
		FPType a_;
		FPType b_;

};
#endif