#ifndef _DESC_DIRECTION_PATH_PG_
#define _DESC_DIRECTION_PATH_PG_

#include "DescDirectionPath.h"

/** \brief This class implements direction of descent of projected gradient algorithm.
	\details For details see \note MichaelFlorian2009.
	\note This implementation adds special technique in order to keep direction of descent
	consistent with flow conservation constraints. This allows the algorithm
	to reach high precision.
*/
class DescDirectionPathPG : public DescDirectionPath {
	public:
		DescDirectionPathPG(FPType delta);
		~DescDirectionPathPG();
		PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths,
			bool &isEquilibrated);
		
};

#endif
