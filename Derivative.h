#ifndef _DERIVATIVE_
#define _DERIVATIVE_

#include "UsedTypes.h"

class LinkFncContainer;
class LinkFnc;

/** \brief This class implements calculation of directional derivative of the objective function with 
	respect to step size required in line search.
*/
class Derivative {
	public:
		/** @param zeroFlow link flow tolerance.
			@param fnc container of link cost functions.
		*/
		Derivative(FPType zeroFlow, LinkFncContainer *fnc);
		virtual ~Derivative();
		
		/** Сalculates the derivative of objective function with respect to
			step size alpha. 
			@param alpha step size.
			@return calculated derivative.
		*/
		virtual FPType calculate(FPType alpha) const;
		
		/** Sets the pointers to data needed to calculate the derivative.
			This method must be called before calculate() 
			each time the pointers to data change.
			@param size size of the arrays with data.
			@param x array with current link flows.
			@param y array with current direction of descent in terms of link flows.
			@param indexes array with proper indexes that will be used in order to 
			iterate through arrays (x[indexes[i]]), so array x and y can be bigger than size 
			specified in input. 
		*/
		void setDataPointers(int size, FPType *x, FPType *y, int *indexes);

		/** @return link flow tolerance.
		*/
		FPType getZeroFlow() const;
		
		/** This method is used by QuadApp because it requires the same information that is stored
			in this class and also derivative can be calculated there in parallel with other data
			needed for step size calculation. That's why method calculate() is not called directly
			in QuadApp class.*/
		int getSize() const;
		
		/** This method is used by QuadApp.
			@return value of x given link index. */
		FPType getX(int index) const;
		/** This method is used by QuadApp.
			@return value of y given link index. */
		FPType getY(int index) const;
		/** This method is used by QuadApp.
			@return link cost function given link index. */
		LinkFnc* getLinkFnc(int index) const;
		
		int getLinkIndex(int index) const;

		
	private:
		int size_;
		FPType* x_;
		FPType* y_;
		int* indexes_;
		const FPType zeroFlow_;
		
		LinkFncContainer* fnc_;

};

#endif
