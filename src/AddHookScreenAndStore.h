#ifndef ADD_HOOK_SCREEN_AND_STORE
#define ADD_HOOK_SCREEN_AND_STORE

#include "AddHookStoreOut.h"
#include "AddHookScreenOut.h"

/** \brief This class is designed to show convergence information on screen and store it 
	in order to print to file after the algorithm is successfully executed. 
*/ 

class AddHookScreenAndStore : public AddHookStoreOut, public AddHookScreenOut {
	public:
		AddHookScreenAndStore(){};
		~AddHookScreenAndStore(){};
		
		/** Prints convergence data on screen and stores it for further use.
			@param timePassed time passed since last measurement.
			@param gap current value of convergence measure.
		*/
		virtual void produceOut(FPType timePassed, FPType gap) {
			AddHookScreenOut::produceOut(timePassed, gap);
			AddHookStoreOut::produceOut(timePassed, gap);
		};
};

#endif
