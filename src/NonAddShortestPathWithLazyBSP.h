#ifndef NON_ADD_SHORTEST_PATH_WITH_LAZY_BSP
#define NON_ADD_SHORTEST_PATH_WITH_LAZY_BSP 

#include "NonAddShortestPath.h"

/** \brief Non-additive lazy shortest path.

	For details, see \cite Chen1998.
*/
class NonAddShortestPathWithLazyBSP : public NonAddShortestPath {
	public:
		NonAddShortestPathWithLazyBSP(NonAdditivePC *pathCost, int nbNodes, OneSourceBiObjShPath* oneSorceSP, 
							ODMatrix* mat);
		~NonAddShortestPathWithLazyBSP();

		virtual void calculate(int originIndex, int destIndex, int odPairIndex);
		virtual void createPath() const;

	private:
		int prevOriginIndex_;

		void updateLabelTravelTimes(const BiObjLabelContainer& labels);
		FPType recalculatePathTime(BiObjLabel* destLabel) const;

};
#endif