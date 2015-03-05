#ifndef NON_ADD_SHORTEST_PATH_FOR_AON
#define NON_ADD_SHORTEST_PATH_FOR_AON 

#include "NonAddShortestPath.h"

/** \brief Implements non-additive shortest path for all-or-nothing.

	Non-additive one-source shortest path does not store paths that are need for all-or-nothing.
	The responsibility of this class is to create paths for every O-D pair once non-additive 
	shortest path is calculated.
*/
class NonAddShortestPathForAON : public NonAddShortestPath {
	public:
		NonAddShortestPathForAON(NonAdditivePC *pathCost, int nbNodes, OneSourceBiObjShPath* oneSorceSP, 
							ODMatrix* mat);
		~NonAddShortestPathForAON();

		virtual StarLink* getInComeLink(int destIndex) const;

	protected:
		/** Stores minimal cost label minCostLabel into internal data structure.
		*/
		virtual void doSomethingWithMinCostLabel(BiObjLabel* minCostLabel);

	private:
		std::vector<BiObjLabel*> minCostLabels_;
		mutable bool shoulResetPath_;
		mutable std::list<StarLink* > currPath_;
		mutable std::list<StarLink* >::const_iterator linkIt_;


};

#endif