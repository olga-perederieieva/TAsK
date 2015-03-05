#ifndef PAS_WITH_STEP
#define PAS_WITH_STEP

#include "PAS.h"

class LineSearch;

/** \brief This class extends PAS class by adding the possibility to calculate flow shift 
	through a line search.
*/
class PASWithStep : public PAS {
public:
	PASWithStep(FPType zeroFlow, FPType dirTol, LineSearch* lineSearch, int nbLinks);
	~PASWithStep();

private:
	LineSearch* lineSearch_;
	const int nbLinks_;

	/** @return a flow shift calculated through a line search. 
	*/
	virtual FPType getFlowShift();
};

#endif