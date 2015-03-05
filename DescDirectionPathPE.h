#ifndef _DESC_DIRECTION_PATH_PE_APPONE_
#define _DESC_DIRECTION_PATH_PE_APPONE_

#include "DescDirectionPath.h"

/** \brief This class implements the direction of descent of path equilibration algorithm.
	\details For details see \cite Florian1995
*/
class DescDirectionPathPE : public DescDirectionPath {
	public:
		DescDirectionPathPE(FPType delta);
		~DescDirectionPathPE();
		PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths,
											   bool &isEquilibrated);
		
};

#endif
