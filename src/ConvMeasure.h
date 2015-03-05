#ifndef _CONV_MEASURE_
#define _CONV_MEASURE_

#include "UsedTypes.h"

#include <stdlib.h>
#include <iostream>

/** \brief Abstract base class representing convergence measure. 
*/
class ConvMeasure {
	public:
		
		virtual ~ConvMeasure() {};
		
		/** This method checks global convergence of the algorithm.
			@return true if convergence criterion is met, false otherwise. 
		*/
		bool isConverged() {
			gap_ = calculateGap();
			return (gap_ <= precision_);
		} ;
		
		/** Calls convergence measure calculation without explicit calculation 
			of minimum travel time that is an input parameter.
		*/
		bool isConverged(FPType minTravelTime) {
			gap_ = calculateGap(minTravelTime);
			return (gap_ <= precision_);
		};
		
		FPType getPrecision() const {return precision_;};
		FPType getGap() const {return gap_;};
	
	protected:
		
		/** @param precision desired precision of convergence measure.
		*/
		ConvMeasure(FPType precision) : precision_(precision), gap_(0.0) {};
		
		/** @return value of the calculated convergence measure.
		*/
		virtual FPType calculateGap() = 0;
		/**	Calls calculateGap(). Might be overwritten by derived classes to
			calculate convergence measure value without calculating minimum
			travel time which is possible only for relative gap calculation.
			@return value of the calculated convergence measure.
		*/
		virtual FPType calculateGap(FPType minTravelTime) {return calculateGap();};
		
	private:
		
		const FPType precision_;
		FPType gap_;
};

#endif
