#ifndef BI_DIRECTIONAL_POINT_TO_POINT
#define BI_DIRECTIONAL_POINT_TO_POINT 

#include "LabelSettingPoint2PointBSP.h"

class PathsAdder;

class BoundsCalculatorForBackwardNet;

/** \brief Bi-directional bi-objective point-to-point shortest path.
	\author Benoit Mulocher, Olga Perederieieva
*/
class BiDirectionalPoint2PointBSP : public LabelSettingPoint2PointBSP {
	public:

		/** @param net network.
			@param tolls container with link tolls.
			@param bounds object that implement travel time lower bounds.
			@param pathAdder object for adding known paths to the destination node.
			@param backwardBounds travel time bounds for network with reversed links.
			@param dominanceRule object that implements path cost domination rule.
		*/
		BiDirectionalPoint2PointBSP(StarNetwork& net, const TollContainerType& tolls, 
							BoundsCalculatorForBSPBase& bounds, 
							const PathsAdder& pathAdder,
							BoundsCalculatorForBackwardNet& backwardBounds,
							DominationByPathCostBase& dominanceRule);

		~BiDirectionalPoint2PointBSP();

		/** Builds path from a given label.
			@param labelWithMinCost label to restore path from.
			@param path empty path object where links will be added after method call.
		*/
		virtual void createPath(BiObjLabel *labelWithMinCost, std::list<StarLink*>& path);

	protected:
		
		/** \note Must call parent initLabels() first and return whatever parent returns.
			In addition adds destination label to backward list and to backward labels.
			@param originLabel label contained in the origin node.
			@param labels container of all stored labels.
			@return pointer to the data structure with forward labels used to check stopping
					condition.
		*/
		virtual QueueType* initLabels(BiObjLabel* originLabel, BiObjLabelContainer& labels);
		
		/** Implements stopping condition for bi-directional search.
			@param tmpLabels list with all not yet visited labels.
			@return true if the algorithm has not finished yet, false otherwise.
		*/
		virtual bool stillLookingForLabels(const QueueType& tmpLabels) const;

		/** Changes direction of search.
		    @return reference to backward list if NEW direction is backward,
			otherwise calls parent method and return its return value.
		*/
		virtual QueueType* changeDirectionIfNeeded();

		/** Changes set of labels depending on search direction.
			@param labels set of forward labels.
			@return pointer to backward labels if direction is backward, if direction is not forward,
			returns input set of labels.
		*/
		virtual BiObjLabelContainer* changeLabelsIfNeeded(BiObjLabelContainer* labels);

		/** Depending on direction returns node of forward network or reversed network.*/
		virtual StarNode* beginNode(int owner) const;
		/** Depending on direction returns link of forward network or reversed network.*/
		virtual StarLink* beginLink() const;
		/** Depending on direction returns link of forward network or reversed network.*/
		virtual StarLink* getNextLink() const;

		/** @return true if new label with cost vector [newTime, newToll] is worth exploring,
					false otherwise.
		*/
		virtual bool isWorthAdding(FPType newTime, TollType newToll, int nodeIndex) const;
		/** Combines backward and forward labels that belong to the same path.
			@param newLabel newly generated label.
		*/
		virtual void combine(BiObjLabel* newLabel);
		
		/** Adds label to label set.
		*/
		virtual void addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
				BiObjLabel* label);		
	private:
		
		QueueType backwardList_; /**< list of not yet visited backward labels.*/
		QueueType* forwardList_; /**< list of not yet visited forward labels.*/
		BiObjLabelContainer backwardLabels_; /**< non-dominated backward labels stored for each node.*/
		BiObjLabelContainer* forwardLabels_; /**< non-dominated forward labels stored for each node.*/
		StarNetwork* reversedNet_;
		bool curDirectionIsBackward_; /**< true if backward, false otherwise.*/
		int backwardOriginIndex_; /**< index of the origin node, node indexes in reversed and
								forward networks are the same, but link indexes are not the same.*/
		BoundsCalculatorForBackwardNet& backwardBounds_;

};

#endif
