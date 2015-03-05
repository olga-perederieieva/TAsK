#ifndef _DESC_DIRECTION_PATH_ISP_
#define _DESC_DIRECTION_PATH_ISP_

#include "DescDirectionPath.h"

/** \brief This class implements direction of descent of improved social pressure algorithm.
	\details For details see \cite Kumar2011.
*/
class DescDirectionPathISP : public DescDirectionPath {
	public:
		/** @param slope precision of path derivatives, see \cite Kumar2011.
			@param scaleFact value of \f$\pi\f$, see \cite Kumar2011.
			@param delta direction of descent tolerance.
		*/
		DescDirectionPathISP(FPType slope, FPType scaleFact, FPType delta);
		~DescDirectionPathISP();
		
		PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths,
											   bool &isEquilibrated);
		
	private:
		const FPType slope_;
		const FPType scaleFact_;
		
		/** Calculates threshold used in the algorithm in order to
			divide the sets of paths into two groups. For details see \cite Kumar2011.
		*/
		FPType calculateThreshold(FPType minDist, FPType maxDist) const;

		/** Calculates "path derivative". It is basically the sum of derivatives
			of link cost functions w.r.t. link flow that belong to the path.
		*/
		FPType calculateDerivative(Path* path) const;
};

#endif
