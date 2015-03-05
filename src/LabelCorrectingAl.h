#ifndef LABEL_CORR_AL
#define LABEL_CORR_AL

#include "ShortestPath.h"

class StarNetwork;
class StarNode;

/** \brief This class implements the label correcting algorithm for finding shortest paths from 
    a given origin node to all other nodes of the network. 
	\note Algorithm implements a special feature for traffic assignment:
		shortest paths are not allowed to go through zone nodes.
 	\details For details see \cite Sheffi1985. 
 */
class LabelCorrectingAl : public ShortestPath {
	
	public:
		LabelCorrectingAl(StarNetwork *netPointer);
		~LabelCorrectingAl();
		
		/** Calculates shortest paths from originIndex to all other nodes 
			of the network.
		*/
		virtual void calculate(int originIndex);
		
		/** @return cost of shortest path from the given origin to destIndex.
		*/
		virtual FPType getCost(int destIndex) const;
		/** @return pointer to a link that belongs to the shortest path 
				from a given origin to destIndex and points to destIndex.
		*/
		virtual StarLink* getInComeLink(int destIndex) const;
		
		/** @return index of the origin for which the shortest paths were
				calculated most recently.
		*/
		int getCurrentOrigin() const;

	protected:
		/** Special condition that forbids shortest paths to go through zones.
			@return true if it is allowed to visit curNode.
		*/
		virtual bool proceed(StarNode* curNode, int topNode) const;
	
	private:

		/** Internal utility structure that is usually used to store solution of the shortest path problem.
			Since it is used only in this class definition of this structure is directly in this header.
 		*/
		struct nodeInfo {
			FPType dist; /**< current shortest distance to node*/
			int linkIndex; 
		};
		
		StarNetwork *netPointer_;
		nodeInfo *nodeList_;
		const int nbNodes_;
		int originIndex_;
		
};

#endif
