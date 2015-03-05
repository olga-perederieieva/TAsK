#ifndef SHORTEST_PATH_WITH_RAND_RETURN_WITH_FIXED_PROB
#define SHORTEST_PATH_WITH_RAND_RETURN_WITH_FIXED_PROB 

#include "ShortestPathWithRandomReturn.h"

/** \brief Randomised flow update for the case when probability of calculating point-to-point
	shortest path is fixed.

	For details, see ShortestPathWithRandomReturn.
*/
class ShortestPathWithRandomReturnWithFixedProbability : public ShortestPathWithRandomReturn {
	public:
		/** @param shPath  point-to-point shortest path algorithm that will be called
				with a certain probability.
			@param probability fixed probability of calculating point-to-point 
				shortest path.
		*/
		ShortestPathWithRandomReturnWithFixedProbability(ShortestPath* shPath, FPType probability);
		~ShortestPathWithRandomReturnWithFixedProbability();

	protected:
		virtual FPType getProbability() const;

	private:
		FPType probability_;

};
#endif