#ifndef STAR_LINK_WITH_TOLLS
#define STAR_LINK_WITH_TOLLS 

#include "StarLinkWithReverseLink.h"

/** \brief Implements a special type of link that instead of travel time
	returns constant link toll, but doesn't overwrite getForwardLink() method.
*/
class StarLinkWithTolls : public StarLinkWithReverseLink {
	public:
		StarLinkWithTolls(StarLink* link, TollType toll);
		~StarLinkWithTolls();

		virtual FPType getTime() const;

	private:
		TollType toll_;

};
#endif