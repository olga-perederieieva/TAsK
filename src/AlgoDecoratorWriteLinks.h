#ifndef ALGO_DECOR_WRITE_LINKS
#define ALGO_DECOR_WRITE_LINKS

#include "AlgoDecoratorBase.h"

#include <string>

class StarNetwork;

/** \brief This class decorates EqAlgo with writing link flows to the file.
*/
class AlgoDecoratorWriteLinks : public AlgoDecoratorBase {
	public:
		/** @param algo  algorithm for decoration, is deallocated in this class.
			@param net network
			@param fileName file where link flows will be written.
			@param dir (default value empty string) optional parameter, describes directory where
					file should be written.
		*/
		AlgoDecoratorWriteLinks(DecoratedEqAlgo *algo, StarNetwork *net, const std::string &fileName,
			const std::string& dir = "");
		~AlgoDecoratorWriteLinks();
		
		/** Calls execute of component_ and writes link flows to file.*/
		virtual int execute();
		
		
	private:
		StarNetwork *net_;
		const std::string fileName_;
		const std::string dir_;
};

#endif
