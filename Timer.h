#ifndef TIMER_
#define TIMER_

#include "UsedTypes.h"

#include <ctime>

/** \brief This class represents a timer for measuring CPU time.

	This class simplifies the way to measure elapsed time.
*/
class Timer {
	public:
		Timer();
		~Timer();
		
		/** Launches the timer. It can be called as many times as needed
			in order to restart the timer. If this method has never been called starting point 
			of time will be creation of Timer object.
		*/
		void start();
		
		/** @return how much time has passed (in seconds) since either object creation 
			or last call to start() method.
		*/
		FPType getTimePassed() const;
		
	private:
		clock_t start_;
		
};

#endif
