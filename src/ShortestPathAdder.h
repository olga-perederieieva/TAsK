#ifndef SHORTEST_PATH_ADDER
#define SHORTEST_PATH_ADDER 

#include "CurrentPathsAdder.h"

/** \brief Implements addition of known paths techniques for the case
	of adding one paths with minimal non-additive path cost from the set of paths.
*/
class ShortestPathAdder : public CurrentPathsAdder {
	public:
		ShortestPathAdder(const TollContainerType& tolls);
		~ShortestPathAdder();

		void uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const;

};
#endif