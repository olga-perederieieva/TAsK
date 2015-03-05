#ifndef _ADDITIVE_PATH_COST_
#define _ADDITIVE_PATH_COST_

#include "PathCost.h"

/** \brief Additive path cost - sum of the costs of the links belonging to a given path.*/
class AdditivePC : public PathCost {
	public:
		AdditivePC();
		~AdditivePC();
		
		/** Calculates additive path cost.
			@param path path.
			@param odIndex index of O-D pair.
		*/
		FPType calculate(Path *path, int odIndex) const;
};

#endif
