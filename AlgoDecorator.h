#ifndef _ALGO_DECOR_
#define _ALGO_DECOR_

#include "AlgoDecoratorBase.h"

#include <string>

class AddHookStoreOut;

/** \brief Decorates EqAlgo with writing convergence data previously stored by
	AddHookStoreOut into file. 
*/
class AlgoDecorator : public AlgoDecoratorBase {
	public:
		
		/** @param algo algorithm for decoration.
			@param hook object that contains stored data.
			@param fileName name of file where convergence data will be written.
			@param dir (default value empty string) optional parameter, describes directory where
					file should be written.
		*/
		AlgoDecorator(DecoratedEqAlgo *algo, AddHookStoreOut *hook, const std::string& fileName, 
			const std::string& dir = "");
		~AlgoDecorator();
		
		/** Calls execute of component_ and writes convergence data to file.*/
		virtual int execute();
		
		
	private:
		AddHookStoreOut *hook_;
		const std::string fileName_;
		const std::string dir_;
};

#endif
