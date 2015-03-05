#ifndef INDIFF_CURVE
#define INDIFF_CURVE

#include "UsedTypes.h"

#include <stdlib.h>
#include <string>

/** \brief Abstract class that represents scalarization function for non-linear scalarization 
	of two objectives. 
	\details Scalarization function converts toll to time.
	It must be strictly increasing (for optimisation
	formulation of Larsson \cite Larsson2004)
*/
class IndiffCurve {
	public:
		
		virtual ~IndiffCurve(){};

		/** @return value of the scalarization function evaluated at the given toll*/
		virtual FPType evaluate(TollType toll) =0;

		/** @return description of the scalarization function in string format */
		virtual std::string toString() =0;

		/** Does nothing by default.*/
		virtual void perturbTime(FPType percentage, FPType probOfPerturbation) {};
		/** Does nothing by default.*/
		virtual void perturbToll(FPType percentage, FPType probOfPerturbation) {};
		/** Does nothing by default.*/
		virtual void restoreOriginalCurve() {};

	protected:
		IndiffCurve(){};
};

#endif