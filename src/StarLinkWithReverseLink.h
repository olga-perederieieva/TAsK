#ifndef STAR_LINK_WITH_REVERSE_LINK
#define STAR_LINK_WITH_REVERSE_LINK 

#include "StarLink.h"

/** \brief Implements a special type of link that stores pointer to original link.
*/
class StarLinkWithReverseLink : public StarLink {
	public:
		StarLinkWithReverseLink(StarLink* link);
		~StarLinkWithReverseLink();

		virtual FPType getTime() const;

		virtual StarLink* getForwardLink();

	private:
		StarLink* originalLink_;

};

#endif
