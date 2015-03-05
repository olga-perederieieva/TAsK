#ifndef ASTAR_WITHOUT_ZONE_SKIP
#define ASTAR_WITHOUT_ZONE_SKIP 

#include "Astar.h"

/** \brief Performs usual A* algorithm without skipping zone nodes.
*/
class AstarWithoutZoneSkip : public Astar {
	public:
		AstarWithoutZoneSkip(StarNetwork* netPointer, ODMatrix* odMatrix);
		~AstarWithoutZoneSkip();

	protected:
		/** @return always false.
		*/
        virtual bool shouldSkipZone(StarNode* node, int nodeIndex, int originIndex) const;

};
#endif