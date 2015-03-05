#ifndef PAS_MANAGER_WITH_STEP
#define PAS_MANAGER_WITH_STEP value

#include "PASManager.h"

class LineSearch;

/** \brief This class extends PASManager to allow use of line searches for flow shift calculation.
	\details This class is responsible for allocation of PASWithStep.
*/
class PASManagerWithStep : public PASManager {
public:
	PASManagerWithStep(const ShortestPath &shPath, FPType dirTol, int nbNodes, 
						FPType mu, FPType v,
						int nbLinks, LineSearch* lineSearch, FPType zeroFlow);
	~PASManagerWithStep();

private:
	LineSearch* lineSearch_;
	const int nbLinks_;

	virtual PAS* allocatePAS();
};
#endif