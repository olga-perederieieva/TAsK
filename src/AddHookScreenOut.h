#ifndef ADD_HOOK_SCREEN_OUT
#define ADD_HOOK_SCREEN_OUT

#include "AddHook.h"

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cassert>

/** \brief This class is designed to print convergence information on
	the screen after each iteration of the algorithm.
*/
class AddHookScreenOut : public AddHook {
	public:
		
		/** @param precision (default value is 10)  number of digits to print on the screen.
		*/
		AddHookScreenOut(int precision = 10) : precision_(precision) {
			assert(precision >= 1 && precision < 17);
		};

		virtual ~AddHookScreenOut() {};
		
		/** Prints convergence on screen.
			@param timePassed time passed since last measurement.
			@param gap current value of convergence measure.
		*/
		virtual void produceOut(FPType timePassed, FPType gap){
			std::cout << std::setprecision(precision_) << timePassed << " " << gap << std::endl;
		};
		
	private:
		int precision_;

};

#endif
