#ifndef D_A_GRAPH
#define D_A_GRAPH

#include "UsedTypes.h"

#include <list>

class StarLink;
class StarNetwork;
class DAGraphNode;
class ODMatrix;

/** \brief This class is abstract. It implements basic functionality of 
	directed acyclic graphs (DAG) and provides interface for derived classes 
	to perform flow movements within each DAG. 
*/
class DAGraph {
	
	public:
		
		virtual ~DAGraph();
		
		/** Adds link to the graph if it is not in it yet and allocates corresponding
			DAGraphNode objects if they were not allocated before.
		*/
		bool addLink(StarLink *link);
		
		/** Deletes unused links. A link is unused if the corresponding origin flow is
			less than ZERO_FLOW and the connectivity of the graph is maintained.
		*/
		bool removeUnusedLinks();

		/** Also deletes unused links, but iterates through the list of links passed as a 
			parameter.
		*/
		bool removeUnusedLinks(const std::list<StarLink*> &links); 	

		/** Sets the reference listRef with links that go out of node with index nodeIndex.
		*/
		void getOutLinks(int nodeIndex, std::list<StarLink*>& listRef);

		/** Sets the reference listRef with links that come into node with index nodeIndex.
		*/
		void getInLinks(int nodeIndex, std::list<StarLink*>& listRef);

		/** @return a copy of the list with links that go out of node with index nodeIndex.
		*/
		std::list<StarLink*> getOutLinksCopy(int nodeIndex) const;

		/** @return a copy of the list with links that come into node with index nodeIndex.
		*/
		std::list<StarLink*> getInLinksCopy(int nodeIndex) const;
		
		/** Starts iterating trough all nodes in ascending topological order (i.e. it starts from
			origin).
			@return index of the first node in ascending topological order, -1 if list is empty
		*/
		int beginAscPass();

		/**	@return index of the current node in ascending topological order, -1 if end of list
		*/
		int getNextAscPass();

		/** Starts iterating trough all nodes in descending topological order (i.e. it starts from
			the furthest destination node).
			@return index of the first node in descending topological order, -1 if list is empty
		*/
		int beginDescPass();

		/**	@return index of the current node in descending topological order, -1 if end of list
		*/
		int getNextDescPass();
		
		/** Creates a topological order of DAG nodes.
		*/
		bool topologicalSort();

		/** Assigns a minimum and maximum distance to each node of DAG. 
			It is assumed that topological order exists and the graph was
			topologically sorted beforehand.
			Algorithms stops if the destination node destIndex was reached 
			(as a result min and max
			distance to nodes that are topologically after destIndex are not calculated).
			If these distances must be calculated for all nodes destIndex must be set to -1.
			\warning methods assumes that topological order starts with origin node.
		*/
		void buildMinMaxTrees(int destIndex);

		/** Implements algorithm of addition of better links to the graph in such a way that
			acyclicity is maintained. Algorithm is described in the paper of Nie 
			"A class of bush-based algorithms for the traffic assignment problem",
			Transportation Research, 2010. 
		*/
		bool addBetterLinks();
		
		/** This method must be implemented by derived classes. It must change origin flows and 
			corresponding link flows and travel times by brining the current solution closer to 
			equilibrium.
		*/
		virtual bool moveFlow() = 0;
		
		/** This method is used in OriginSet in order to initialise all bushes and in derived 
			classes	to keep origin flows consistent with performed flow moves.
			Adds demand to internal structure that stores origin flows for each link of the bush.
		*/
		void addOriginFlow(int linkIndex, FPType demand);

		/** This method is used only in PAS class  because it adds link if it was not in the bush 
			when the origin flow was added. 
		*/
		void addOriginFlowAndCreateLink(StarLink* link, FPType demand);

		/** Sets origin flow of link with index index to value flow.
		*/
		void setOriginFlow(int index, FPType flow);

		/** Sets origin flow of link with index index to zero.
		*/
		void setOriginFlowToZero(int linkIndex);

		/** @return origin flow of link specified by linkIndex.
		*/
		FPType getOriginFlow(int linkIndex) const;
		
		/** @return origin index where this bush belongs to.
		*/
		int getOriginIndex() const;
		
		/** Prints on screen relevant information about this bush.
		*/
		void print() const;

		/** Prints on screen origin flows. For debugging.
		*/
		void printOriginFlow() const;
		
		/** For debugging.
		*/
		void printShPath(int node);
		
		/** For debugging.
		*/
		void printMaxShPath(int node);
		
		/** Checks if origin flows satisfy network flow constraints.
			@return maximum deviation of flow.
		*/
		FPType checkOFlowsFeasibility();
		
		/** @return number of links that belong to this bush. 
		*/
		FPType getNbLinksInBush() const;
		
	protected:
		
		// just for iteration without top.order in TAPAS
		int *nodeIndexes_;
		int nodeSize_;
		
		static StarNetwork * net_;
		static ODMatrix *mat_;
	
		static FPType zeroFlow_;

		/** @param net pointer to network.
			@param mat pointer to O-D matrix.
			@param zeroFlow link flow tolerance.
			@param originIndex origin index.
		*/
		DAGraph(StarNetwork *net, ODMatrix *mat, FPType zeroFlow, int originIndex);
		
		/** @return demand of node with index nodeIndex or zero if a given node is not a destination.
		*/
		FPType getDemand(int nodeIndex) const;
		
		/** @param index index of a node that belongs to this DAG. If other index is passed, then
			assertion will execute and program will terminate.
			@return DAGraphNode object. 
		*/
		DAGraphNode* getNode(int index) const;
		
		/** Removes a link specified by index from DAG if the connectivity of DAG is retained. 
			@return true if link was removed, false otherwise (link is not going to be removed
			if DAG is not connected after this operation. 
		*/
		bool removeLink(int index);

		/** Throws an exception if a back edge is detected. This method can be overridden by derived 
			classes, see DAGraphTapas.
		*/
		virtual bool handleBackEdge(StarLink* link);

		/** This method is a hook method for DAGraphTapas. It does nothing if it is not overridden.
		*/
		virtual void handleExploredLink(StarLink* link) {};

		/** This method always returns true if it is not overridden, see DAGraphTapas for details.
		*/
		virtual bool checkPositiveFlow(int linkIndex) {return true;};
		
	private:
		const int originIndex_;
		
		FPType *originFlows_;
		DAGraphNode **nodes_;
		
		StarLink **links_;
		int linkSize_; 
		std::list<int> linkIndexes_;
		
		std::list<int> topOrder_;
		
		// for passes
		std::list<int>::iterator currNode_;
		std::list<int>::reverse_iterator currNodeDesc_;
		
		// for improving set
		std::list<int> p2Cont_;
		
		// for topological sort
		int clock_;
		
		// to keep track of initialisation of static variables
		static bool wasInitialised_; 
		
		/** @return true if link can improve current bush, false otherwise.
		*/
		bool worthAdding(StarLink* link);

		/** Adds links from so-called set P2 to the bush. For details see \cite Nie2010.
		*/
		bool addFromP2();
		
		/** @return link index of the link that is NOT yet in the bush.
			In order to find the first such link starIndex must be set to -1.
		 */
		int getNextNotInSet(int startIndex) const;

		/** @return index of the next link for which both head and tail nodes belong
			to the bush, -1 otherwise.
		*/
		bool isReachable(StarLink* link) const;
		
		/** This method is used in topological sort. For details see \cite Dasgupta2006.
		*/
		bool explore(int vertex, bool *visited);

		/** This method is used in topological sort. For details see \cite Dasgupta2006.
		*/
		inline void preVisit(int vertex);

		/** This method is used in topological sort. For details see \cite Dasgupta2006.
		*/
		inline void postVisit(int vertex);
		
		static void initialiseStaticMembers(StarNetwork *net, ODMatrix *mat, FPType zeroFlow);
		
};

#endif
