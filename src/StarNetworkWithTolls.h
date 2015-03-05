#ifndef NETWORK_WITH_TOLLS
#define NETWORK_WITH_TOLLS 

#include "UsedTypes.h"

class StarNetwork;
class StarLink;

/** \brief Factory class for creating network object where all links are of StarLinkTolls
	from a given network.
*/
class StarNetworkWithTolls {
	public:
		StarNetworkWithTolls(const TollContainerType& tolls);
		~StarNetworkWithTolls();

		StarNetwork* createNetWithTolls(const StarNetwork & net);

	protected:
		virtual StarLink* createLink(StarLink* link);
		

	private:
		const TollContainerType& tolls_;
};
#endif
