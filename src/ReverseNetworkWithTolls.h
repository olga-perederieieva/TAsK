#ifndef REVERSED_NETWORK_WITH_TOLLS
#define REVERSED_NETWORK_WITH_TOLLS 

#include "ReverseNetwork.h"
#include "UsedTypes.h"

/** \brief Factory class for creating reversed network where each link cost is
	constant toll.
*/
class ReverseNetworkWithTolls : public ReverseNetwork {
	public:
		ReverseNetworkWithTolls(const TollContainerType& tolls);
		~ReverseNetworkWithTolls();


	protected:
		virtual StarLink* createLink(StarLink* link);

	private:
		const TollContainerType& tolls_;
};
#endif