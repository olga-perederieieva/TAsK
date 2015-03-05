#ifndef BI_OBJ_LABEL_CONT_PATH
#define BI_OBJ_LABEL_CONT_PATH

#include "BiObjLabel.h"

#include <list>
#include <vector>

class BiObjLabel;

typedef std::list<BiObjLabel*>  NodeContainerType;
typedef NodeContainerType::iterator LabelsIterator;

/** \brief Container of bi-objective labels.
*/
class BiObjLabelContainer {
	public:
		
		/** @param nbNodes total number of nodes in the network.
		*/
		BiObjLabelContainer(int nbNodes);
		~BiObjLabelContainer();

		/** @param nodeIndex index of the node to iterate its labels.
			@return iterator of labels that belong to nodeIndex.
		*/
		LabelsIterator begin(int nodeIndex) const;
		
		/** @return end of iterator of labels that belong to nodeIndex.
		*/
		LabelsIterator end(int nodeIndex) const;

		/** Erases label that iterator it points to from the list of labels 
			that belong to nodeIndex.
			@return iterator after erasing element.
		*/
		LabelsIterator erase(int nodeIndex, LabelsIterator& it);

		/** @return number of labels that belong to node with index nodeIndex.
		*/
		int getNbLabels(int nodeIndex) const;

		/** @return true if set of labels that belong to node nodeIndex is empty,
			false otherwise.
		*/
		bool empty(int nodeIndex) const;
		
		/** Adds label to the end of label container of node nodeIndex.
		*/
		void pushBackLabel(int nodeIndex, BiObjLabel* label);

		/** Deletes all labels that belong to node nodeIndex.
		*/
		void clearNodeLabels(int nodeIndex);

		/** Prints on screen all available labels of every node.
		*/
		void print() const;

		/** Deletes all labels.
			\warning Method does not work if two same pointers to BiObjLabel were added
			to the container at some point.
		*/
		void deleteAllLabels();

		/** @return number of nodes in the network.
		*/
		int getNbNodes() const {
			return nbNodes_;
		};

	private:
		const int nbNodes_;
		mutable std::vector<NodeContainerType> labels_;

};

#endif