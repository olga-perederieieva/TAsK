#ifndef STAR_LINK_TOLLS
#define STAR_LINK_TOLLS 

#include "StarLink.h"

/** \brief Implements a special type of link that instead of travel time
	returns constant link toll.
*/
class StarLinkTolls : public StarLink {
	public:
		StarLinkTolls(StarLink* link, TollType toll);
		~StarLinkTolls();
		/** @return constant link toll.
		*/
		virtual FPType getTime() const;
		/** @return pointer to the original link that calculates proper link travel time.
		*/
		virtual StarLink* getForwardLink();
		
	private:
		TollType toll_;
		StarLink* originalLink_;
};
#endif
