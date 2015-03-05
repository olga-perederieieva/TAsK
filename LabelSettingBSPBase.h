#ifndef LABEL_CORRECTING_BSP_BASE
#define LABEL_CORRECTING_BSP_BASE

#include "BiObjLabelContainer.h"
#include "StarNetwork.h"
#include "LexOptComparison.h"

#include <set> 

typedef std::multiset<BiObjLabel*, LexOptComparison > QueueType;

/** \brief Abstract base class that implements main steps of the one-source
	bi-objective label-setting algorithm.
	\note Unlike single-objective case efficient paths are allowed to go through zones.
*/
class LabelSettingBSPBase {
	public:
		
		virtual ~LabelSettingBSPBase() = 0;

	protected:

		StarNetwork& net_;
		const TollContainerType& tolls_;

		/** @param net network.
			@param tolls container with link tolls.
		*/
		LabelSettingBSPBase(StarNetwork& net, const TollContainerType& tolls);

		/** Implements general structure of one source bi-objective label-setting
			algorithm to calculate non-dominated labels from originIndex.
		*/
		void calculateBSP(int originIndex, BiObjLabelContainer& labels); 

		/** @return true if stopping condition is  not yet met, false otherwise.
		*/
		virtual bool stillLookingForLabels(const QueueType& tmpLabels) const;

		/** Finds the next label that must be visited and removes it from the
			list of labels.
			@return pointer to the next label that must be visited.
		*/
		virtual BiObjLabel* getNextLabelAndRemoveItFromList(QueueType& tmpLabels);

		/** Hook method for children classes.
			@return true.
		*/
		virtual bool isWorthAdding(FPType newTime, TollType newToll, int nodeIndex) const {
			return true;
		};

		/** Initialises set of potentially efficient labels and set of non-dominated labels
			for every node.
		*/
		virtual QueueType* initLabels(BiObjLabel* originLabel, BiObjLabelContainer& labels);

		/** Does nothing. This methods is redefined in bidirectional search.
		*/
		virtual QueueType* changeDirectionIfNeeded();
		/** Does nothing. This methods is redefined in bidirectional search.
		*/
		virtual BiObjLabelContainer* changeLabelsIfNeeded(BiObjLabelContainer* labels);
		/** @return pointer to node with index owner.
		*/
		virtual StarNode* beginNode(int owner) const;
		/** @return pointer to link that goes out of the node returned by beginNode.
		*/
		virtual StarLink* beginLink() const;
		/** @return pointer to the next link that goes out of the node returned by beginNode.
		*/
		virtual StarLink* getNextLink() const;

		/** Adds label to set of labels that belong to nodeIndex.
		*/
		virtual void addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, BiObjLabel* label);
		/** Hook method for children classes. Does nothing by default.
		*/
		virtual void uploadPaths();
		/** Hook method for children classes. Does nothing by default.
		*/
		virtual void combine(BiObjLabel* newLabel);

		/** @return true is label specified by newTime and newToll is not dominated
				by any label that belongs to node linkHead, false otherwise.
		*/
		bool isNonDominated(FPType newTime, TollType newToll, const BiObjLabelContainer& labels,
				int linkHead) const;

		/** Goes through all labels that belong to node linkHead and removes any of them that
			are dominated by label. Adds label to set of labels belonging to node linkHead.
		*/
		void mergeLabels(BiObjLabel* label, BiObjLabelContainer& labels, int linkHead,
				QueueType& tmpLabels);

	private:

		QueueType tmpLabels_;
		std::vector<StarLink*> seenLinks_;

		/** Allocates memory for a new label.
			@return newly created label.
		*/
		BiObjLabel* createLabel(BiObjLabel* currentLabel, StarLink* link) const;
		/** Deletes labelInNode from tmpLabels if it is there. Implementation
			is complicated, because all labels are sorted lexicographically, but
			we need to remove exactly labelInNode, not any other label that happened
			to have the case travel time value.
		*/
		void deleteFromTmpLabels(QueueType& tmpLabels, BiObjLabel* labelInNode);
};
#endif
