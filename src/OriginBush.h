#ifndef _ORIGIN_BUSH_
#define _ORIGIN_BUSH_

#include "UsedTypes.h"

class DAGraph;
class PairOD;
class ODMatrix;
class StarNetwork;
class StarLink;

/** \brief This class implements a bush - basic element of origin-based algorithms.
	\details This class implements a factory method - children classes allocate necessary 
	DAGraph objects. DAGgraph is not used directly but through OriginBush because  
	some operations are common (so it reduces code duplication) also some of 
	the operations can be over-ridden by children classes.
	For example, see OriginBushTapas. */
class OriginBush {
	
	public:
		
		/** This class is responsible for deallocating DAGraph object.
		*/
		virtual ~OriginBush();
		
		/** Allocates memory for DAGraph. 
			\note It MUST be called after each OriginBush creation.
		*/
		void allocateDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow, FPType dirTol);
		
		/** Recalculates topological order if the bush changed during previous iteration.
		*/
		virtual void updateTopSort();
		
		/** Performs flow moves according to some origin-based algorithm. 
			All new flows are assigned to StarNetwork 
			object and link travel times are updated
			(responsibility of doing this belongs to DAGraph class). 
		*/
		bool equilibrate();
		
		/** Tries to improve current bush by adding better links. Topological sorting and 
			building of min- and max-trees are also performed if new links were added to the bush.
			@return true if current bush was improved, false otherwise.
		*/
		virtual bool improve();
		
		/** Removes unused links from the bush. A link is unused if its flow is less than
			a predefined constant ZERO_FLOW.
		*/
		virtual void removeUnusedLinks();
		
		/** Adds link \b link to the bush. This method is also responsible for updating 
			origin flows and link flows. This method is called in OriginSet in order to 
			initialise the bush.
		*/
		void addLinkAndUpdateFlow(StarLink *link, PairOD* dest);
		
		/** Prints bush on the screen.
		*/
		void print();
		/** @return origin index.
		*/
		int getOriginIndex() const;
		/** For debugging. Prints origin flows on the screen.
		*/
		void printOriginFlow() const;
		
		/** For debugging. Checks if origin flows are feasible.
			@return maximum deviation of flow.
		*/
		FPType checkOFlowsFeasibility(); 
		
	protected:
		OriginBush();
			
	private:
		
		DAGraph *daGraph_;
		bool topSortUpToDate_;
		
		/** Factory method used for DAGraph creation.
		*/
		virtual DAGraph* createNewDAG(int index, StarNetwork *net, ODMatrix *mat, FPType zeroFlow, 
			FPType dirTol) = 0;

		/** Calls buildMinMaxTrees() in improve() method. If it is not necessary, 
			this method can be over-ridden by children classes. For example, see OriginBushLUCE.
		*/
		virtual void callBuildMinMaxTrees();
		
};

#endif
