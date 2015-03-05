#ifndef PATH_COST
#define PATH_COST

#include "UsedTypes.h"

class Path;

/** \brief This class is an interface for path cost calculation. 
*/
class PathCost {
	public:
		virtual ~PathCost(){};
		/** @param path object that stores path.
			@param odIndex O-D pair index.
		*/
		virtual FPType calculate(Path *path, int odIndex) const = 0;
	protected:
		PathCost(){};
};

#endif
