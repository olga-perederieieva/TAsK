#ifndef PATH_BASED_ALGO_EQII
#define PATH_BASED_ALGO_EQII

#include "PathBasedAlgo.h"

/** \brief  This class implements Equilibration II for path-based algorithms.
	\warning Equilibration II might not converge in case of high precision.
*/
class PathBasedAlgoEqII : public PathBasedAlgo {
public:
	/** @param pathSet set of all paths.
		@param component object that performs extra operations with convergence.
		@param conv convergence measure.
		@param net network.
		@param timeLimit maximum allowed execution time of algorithm in seconds.
		@param maxNbIter maximum number of iteration of Equilibration II.
		@param mat O-D matrix.
	*/
	PathBasedAlgoEqII(PathSet *pathSet, AddHook *component, ConvMeasure *conv, 
					StarNetwork *net, FPType timeLimit, 
					int maxNbIter, ODMatrix* mat);
	~PathBasedAlgoEqII();

private:
	const int maxNbIter_;

	virtual bool mainLoop(ODSet *odSet);

};
#endif