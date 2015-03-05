#ifndef ORIGIN_BUSH_B
#define ORIGIN_BUSH_B

#include "OriginBush.h"

#include <stdlib.h>

class LineSearch;

/** \brief This class implements creation of DAGraphB and DAGraphBWithStep.
*/
class OriginBushB : public OriginBush {
	public:
		/** @param useMultiStep true if multiple Newton steps must be applied, false otherwise.
			@param lineSearch (default value is NULL) if it is not given, then DAGraphB will be 
			created, if it is given and not NULL DAGraphBWithStep will be created.
		*/
		OriginBushB(bool useMultiStep, LineSearch* lineSearch = NULL);
		~OriginBushB();
		
	private:
		static bool useMultiStep_;
		static LineSearch* lineSearch_;

		virtual DAGraph* createNewDAG(int index, StarNetwork *net, ODMatrix *mat,
									  FPType zeroFlow, FPType dirTol);
};

#endif
