#ifndef DESC_DIRECTION_PATH_PE_SCALED_
#define DESC_DIRECTION_PATH_PE_SCALED_

#include "DescDirectionPath.h"

/** \brief This class implements a scaled direction of descent for path-based algorithms.
	\details Scaled direction means that usual direction of descent is divided by second derivative 
	of the objective function with respect to flow move. Scaled direction is usually applied in 
	approach 2 and 3 (These approaches are implemented for algorithms PE and GP, see PathAlgoCreator).
*/
class DescDirectionPathScaled : public DescDirectionPath {
	public:
		/** @param pathDir object that implements direction of descent calculation,
					this direction will be scaled.
			@param nbLinks number of links in the network.
			@param delta direction of descent tolerance.
		*/
		DescDirectionPathScaled(DescDirectionPath *pathDir, int nbLinks, FPType delta); 
		
		/** This class is responsible for destruction of pathDir. */
		~DescDirectionPathScaled();
		
		/** \note It is assumed that positive value of direction is always the last one in paths.
		*/
		PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths,
							bool &isEquilibrated);

		
	private:
		DescDirectionPath *pathDir_;
		const int nbLinks_;
};

#endif
