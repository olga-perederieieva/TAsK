#ifndef AON_NON_ADDITIVE
#define AON_NON_ADDITIVE value

#include "AONUsual.h"

class NonAddShortestPathForAON;

/** \brief All-or-nothing procedure for non-additive case.
*/
class AONNonAdditive : public AONUsual {
	public:
		AONNonAdditive(const ODMatrix &mat, NonAddShortestPathForAON* shPath);

		/** \warning deallocates memory for shPath.*/
		~AONNonAdditive();

	protected:
		/** In the non-additive case, we skip on calculating total minimum time 
			for a given O-D pair.
			@return always zero.
		*/
		virtual FPType calculateTravelTime(const PairOD& dest) const;
		
};

#endif