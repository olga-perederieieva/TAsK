#ifndef ORIGIN_BUSH_LUCE
#define ORIGIN_BUSH_LUCE

#include "OriginBush.h"

class DAGraphLUCE;
class LineSearch;

/** \brief This class is responsible for DAGraphLUCE creation. 
	\details It also over-rides callBuildMinMaxTrees() method in order to avoid unnecessary 
	calculation of min- and max-trees.
*/
class OriginBushLUCE : public OriginBush {
	public:
		explicit OriginBushLUCE(LineSearch* lineSearch);
		~OriginBushLUCE();
		
		/** Calls OriginBush improve() method and additional method to prepare data specific 
			to LUCE algorithm.
		*/
		virtual bool improve();
		
	private:
		DAGraphLUCE* dag_;
		static LineSearch* lineSearch_;

		virtual DAGraph* createNewDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow,
									  FPType dirTol);
		
		/** Does noting because LUCE needs only topological order and not min- or
			max-trees.
		*/
		virtual void callBuildMinMaxTrees(){};
};

#endif
