#ifndef QUADRATIC_APP
#define QUADRATIC_APP

#include "LineSearch.h"

class StarNetwork;

/** \brief This class implements quadratic approximation for step size calculation.
*/
class QuadApp : public LineSearch {
	
	public:
		
		/** @param der derivative calculation.
			@param net network.
		*/
		QuadApp(Derivative *der, StarNetwork* net);
		~QuadApp();
	
		virtual FPType execute(FPType a, FPType b);

	private:
		StarNetwork* net_;

};

#endif

