#ifndef NON_ADDITIVE_SH_PATH
#define NON_ADDITIVE_SH_PATH

#include "ShortestPath.h"

class NonAdditivePC;
class OneSourceBiObjShPath;
class Point2PointBiObjShPath;
class ODMatrix;
class BiObjLabel;
class BiObjLabelContainer;

#include <map>
#include <list>

/** \brief This class implements non-additive shortest path algorithm.
	\details It is based on bi-objective shortest path and indifference 
	curves that are encapsulated in NonAdditivePC.
*/
class NonAddShortestPath : public ShortestPath {

	public:
		NonAddShortestPath(NonAdditivePC *pathCost, int nbNodes, OneSourceBiObjShPath* oneSorceSP, 
							Point2PointBiObjShPath* point2pointSP,
							ODMatrix* mat);
		~NonAddShortestPath();

		virtual void calculate(int originIndex);
		virtual void calculate(int originIndex, int destIndex, int odPairIndex);
		virtual FPType getCost(int destIndex) const;

		/** \note Current implementation takes linear time in number of links 
			belonging to this path.
		*/
		virtual StarLink* getInComeLink(int destIndex) const;

	protected:
		std::vector<FPType> costs_;
		mutable std::list<StarLink*> path_;
		OneSourceBiObjShPath* oneSorceSP_;
		int curDestIndex_;
		BiObjLabel* minCostLabel_;
		NonAdditivePC *pathCost_;
		
		/** Calculates non-additive cost for a given destination based on last
			call to non-additive shortest path.
			@return pair path cost and pointer to label with this cost.
		*/
		std::pair<FPType, BiObjLabel*> calculateCostToDest(int destIndex, int odPairIndex, 
			const BiObjLabelContainer& labels);

		/** Build path.
		*/
		virtual void createPath() const;
		/** Hook method for derived classes. Does nothing.
		*/
		virtual void doSomethingWithMinCostLabel(BiObjLabel* minCostLabel);

	private:
		
		Point2PointBiObjShPath* point2pointSP_;
		ODMatrix* mat_;
		mutable std::list<StarLink*>::const_iterator currLink_;
		
			
};
#endif
