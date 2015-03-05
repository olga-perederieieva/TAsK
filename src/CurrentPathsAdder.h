#ifndef CURRENT_PATHS_ADDER
#define CURRENT_PATHS_ADDER 

#include "PathsAdder.h"
#include "UsedTypes.h"

class PathSet;

/** \brief Implements addition of known paths techniques for the case
	of adding all paths from the set of paths.
*/
class CurrentPathsAdder : public PathsAdder {
	public:
		/** @param tolls tolls container.
		*/
		CurrentPathsAdder(const TollContainerType& tolls);
		~CurrentPathsAdder();

		/** Stores a pointer to the set of all paths with positive flow.
			\note Must be called once after object creation.
			Cannot be called in the constructor because PathSet object uses
			this class indirectly trough other classes.
		*/
		void setPathSet(PathSet* paths);

		/** Adds labels corresponding to known paths to the destination node destIndex.
		*/
		virtual void uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const;

	protected:
		PathSet* paths_;
		const TollContainerType& tolls_;

};
#endif