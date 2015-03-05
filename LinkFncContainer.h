#ifndef _LINK_FNC_CONTAINER_
#define _LINK_FNC_CONTAINER_

class LinkFnc;
class StarNetwork;

#include <vector>

typedef std::vector<LinkFnc*>::iterator LinkIterator;

/** \brief This class is a container of link cost functions.
	\details It is used in Derivative for faster access. Alternatively, all 
	link cost functions are also stored in StarLink objects.
*/
class LinkFncContainer {
	public:
		LinkFncContainer(StarNetwork *net);
		~LinkFncContainer();
		
		/** @return iterator to the first element of container.*/
		LinkIterator begin();
		/** @return end of container.*/
		LinkIterator end();
		
		/** @return pointer to a link cost function with link index \b index.*/
		LinkFnc* getLinkFnc(int index);
		
	private:
		std::vector<LinkFnc*> links_;
};

#endif
