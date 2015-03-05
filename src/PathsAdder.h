#ifndef PATHS_ADDER
#define PATHS_ADDER 

class BiObjLabelContainer;

/** \brief Base class for the addition of known path speed-up technique for bi-objective label-setting
	algorithm.
*/
class PathsAdder {
	public:
		virtual ~PathsAdder() {};
		/** Uploads known path to the destination node.
			@param odIndex O-D pair index.
			@param destIndex destination index.
			@param labels container with non-dominated labels.
		*/
		virtual void uploadPaths(int odIndex, int destIndex, BiObjLabelContainer& labels) const = 0;

	protected:
		PathsAdder() {};

};
#endif