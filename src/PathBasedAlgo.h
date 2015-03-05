#ifndef _PATH_BASED_ALGO_
#define _PATH_BASED_ALGO_

#include "EqAlgo.h"
#include "PathSetPrinterToFile.h"

class PathSet;

/** \brief This class implements framework for path-based algorithms.
*/
class PathBasedAlgo : public EqAlgo {
	
	public:
		/** @param pathSet set of all paths.
			@param component object that performs extra operations with convergence.
			@param conv convergence measure.
			@param net network.
			@param timeLimit maximum allowed execution time of algorithm in seconds.
			@param mat O-D matrix.
		*/
		PathBasedAlgo(PathSet *pathSet, AddHook *component, ConvMeasure *conv, 
					StarNetwork *net, FPType timeLimit, 
					ODMatrix* mat); 
		virtual ~PathBasedAlgo();
		
	protected:
		/** Performs main operation made during one iteration for a given O-D pair.
			It implements equilibration I. Equilibration II over-rides this method.
			For details see PathBasedAlgoEQII.
			@return true if current O-D pair was equilibrated, false otherwise.
			The return value is needed only for equilibration II.
		*/
		virtual bool mainLoop(ODSet *odSet);

	private:
		PathSet *pathSet_;
		ODMatrix* mat_;
		PathSetPrinterToFile printPathSet_;

		virtual void initialise();
		virtual void performOneIteration();
		
};

#endif
