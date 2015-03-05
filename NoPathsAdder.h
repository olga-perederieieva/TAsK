#ifndef NO_PATHS_ADDER
#define NO_PATHS_ADDER 

#include "PathsAdder.h"

/** \brief Implements a situation when no paths are added to the destination node
	for bi-objective label-setting algorithm.
*/
class NoPathsAdder : public PathsAdder {
	public:
		NoPathsAdder();
		~NoPathsAdder();

		/** Does nothing.
		*/
		virtual void uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const;

};
#endif