#ifndef SHORTEST_PATH_WITH_RANDOM_RETURN
#define SHORTEST_PATH_WITH_RANDOM_RETURN 

#include "ShortestPath.h"

/** \brief Implements randomised flow update strategy for path-based algorithms.

	Some of the point-to-point shortest path calculations are skipped. The probability
	of performing a point-to-point shortest path calculation depends on the current 
	iteration: it is equal to one for the first iteration and to \f$0.1 + 1/iter\f$.
	This class works with any kind of point-to-point shortest path: A*, non-additive
	shortest path, etc.
*/
class ShortestPathWithRandomReturn : public ShortestPath {
	public:
		/** @param shPath  point-to-point shortest path algorithm that will be called
				with a certain probability.
		*/
		ShortestPathWithRandomReturn(ShortestPath* shPath);
		~ShortestPathWithRandomReturn();

		virtual void calculate(int originIndex);  
		virtual void calculate(int originIndex, int destIndex, int odPairIndex); 
		virtual FPType getCost(int destIndex) const;
		virtual StarLink* getInComeLink(int destIndex) const;

	protected:
		/** @return probability of calculating point-to-point shortest path.
		*/
		virtual FPType getProbability() const;

	private:
		ShortestPath* shPath_;
		bool returnInf_;
		int nbIters_;

};
#endif