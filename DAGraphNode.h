#ifndef _DAGRAPH_NODE_
#define _DAGRAPH_NODE_

#include "StarLink.h"

#include <list>

/** \brief This class is a basic element in DAGraph. It represents a node of the graph.
	\details All variables are public for faster access.
*/
class DAGraphNode {
	public:
		
		DAGraphNode(int index, FPType demandVal) : minLink(NULL), maxLink(NULL),
					demand(demandVal), nodeIndex(index), 
					pre(0), post(0), minDist(0.0), maxDist(0.0){};
		~DAGraphNode(){};
		
		/** In-coming links of this node.*/
		std::list<StarLink*> incomeLinks;
		/** Out-going links of this node.*/
		std::list<StarLink*> outLinks;
		
		/** Pointer to a links that belongs to the shortest path ending at this node.*/
		StarLink* minLink;
		/** Pointer to a links that belongs to the longest path ending at this node.*/
		StarLink* maxLink;
		/** Demand of this node, zero if this not is not a destination. 
			It is stored explicitly for faster access.
		*/
		const FPType demand;
		/** Index of this node.*/
		const int nodeIndex;
		/** Special number used in topological sort. It represents when visit of this node started. 
			See \cite{Dasgupta2006}. */
		int pre;
		/** Special number used in topological sort. It represents when visit of this node finished. 
			See \cite{Dasgupta2006}. */
		int post;
		/** Shortest distance to this node.*/
		FPType minDist;
		/** Longest distance to this node. */
		FPType maxDist;
};

#endif
