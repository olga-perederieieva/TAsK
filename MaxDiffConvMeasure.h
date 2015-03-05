#ifndef MAX_DIFF_CONV_MEASURE
#define MAX_DIFF_CONV_MEASURE

#include "ConvMeasure.h"

class ShortestPath;
class ODMatrix;
class PathSet;

/** \brief MaxDiff convergence measure.
*/
class MaxDiffConvMeasure : public ConvMeasure {
	public:
		MaxDiffConvMeasure(FPType precision, ShortestPath* shPath, 
						ODMatrix* mat, PathSet* pathSet);
		~MaxDiffConvMeasure();

	private:
		
		ShortestPath* shPath_;
		ODMatrix* mat_;
		PathSet* pathSet_;

		/** @return values of MaxDiff convergence measure.
		*/
		virtual FPType calculateGap();

		/** @return cost of longest path among paths belonging to O-D pair with index odIndex.
		*/
		FPType calculateMaxPathCost(int odIndex);
};

#endif