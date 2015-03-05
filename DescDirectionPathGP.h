#ifndef _DESC_DIRECTION_PATH_GP_
#define _DESC_DIRECTION_PATH_GP_

#include "DescDirectionPath.h"

/** \brief This class implements direction of descent of gradient projection algorithm.
	\details For details see \cite Chen1998.
*/
class DescDirectionPathGP : public DescDirectionPath {
	public:
		DescDirectionPathGP(FPType delta);
		~DescDirectionPathGP();

		PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths,
											   bool &isEquilibrated);
};

#endif
