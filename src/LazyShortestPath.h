#ifndef LAZY_SHORTEST_PATH
#define LAZY_SHORTEST_PATH 

#include "ShortestPath.h"

/** \brief Lazy shortest path for single-objective shortest path algorithms.

	For details, see \cite Chen1998.
*/
class LazyShortestPath : public ShortestPath {
	public:
		LazyShortestPath(ShortestPath* shPath);
		~LazyShortestPath();

		virtual void calculate(int originIndex);  

		virtual void calculate(int originIndex, int destIndex, int odPairIndex); 
		
		virtual FPType getCost(int destIndex) const;
		
		virtual StarLink* getInComeLink(int destIndex) const; 

	private:
		ShortestPath* shPath_;
		int prevOriginIndex_;
		bool originChanged_;
		FPType newPathCost_;

		/** Goes through the stored shortest path to the destination destIndex
			and updates shortest path distance that might have changed after
			flow shifts
		*/
		void updatePathCost(int destIndex);

};
#endif