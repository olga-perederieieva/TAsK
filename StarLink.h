#ifndef STAR_LINK
#define STAR_LINK

#include "LinkFnc.h"

/** \brief This class is a basic element of StarNetwork. It represents a link of the graph.
*/
class StarLink {
	public:
		/** @param nodeFrom node ID where link comes from.
			@param nodeTo node ID where link points to.
			@param fnc link cost function.
		*/
		StarLink(int nodeFrom, int nodeTo, LinkFnc *fnc);
		virtual ~StarLink();
		
		/** @return ID (ID is obtained from a file with network data) of a node
				where this link comes from.
		*/
		int getNodeFrom() const;
		
		/** @return ID (ID is obtained from a file with network data) of a node
				where this link leads to.
		*/
		int getNodeTo() const;
		
		/** @return current travel time of this link.
			\note It is not automatically updated when flow changes
				(motivation for this is the fact that some of the algorithms now better
				when to update travel time).
		*/
		virtual FPType getTime() const;

		/** @return this pointer unless overridden by derived classes.
		*/
		virtual StarLink* getForwardLink();

		/** @return pointer to a link cost function - in some cases the derivative of the function 
			must be evaluated without physically changing link flow.
		*/
		LinkFnc* getLinkFnc() const;
		
		/** @return internal index (NOT ID) of a node where this link comes from.
		*/
		int getNodeFromIndex() const;
		/** @return internal index (NOT ID) of a node where this link leads to.
		*/
		int getNodeToIndex() const;
		
		/** Sets internal node index of a node where this link comes from.
		*/
		void setNodeToIndex(int index);
		/** Sets internal node index of a node where this link leads to.
		*/
		void setNodeFromIndex(int index);
		
		/** @return current link flow.
		*/
		FPType getFlow() const;
		/** Sets current link flow.
		*/
		void setFlow(FPType flow);
		/** Sets link flow to zero.
		*/
		void setFlowToZero();
		
		/** Adds a specified amount of flow \b flow to current link flow.
		*/
		void addFlow(FPType flow);
		/** @return internal link index.
		*/
		int getIndex() const;
		/** Sets internal link index.
		*/
		void setIndex(int index);
		
		/** Updates current link travel time value and the value of link cost function
			derivative.
		*/
		void updateTime();

		/** @return string representation of link.
		*/
		std::string toString() const;

		/** @return current link cost function derivative value.
		*/
		FPType getDerivative() const;
		
	private:
		int index_;
		const int nodeFrom_;
		const int nodeTo_;
		int nodeFromIndex_;
		int nodeToIndex_;
		FPType flow_;
		FPType time_;
		LinkFnc *fnc_;
		FPType der_;
};
#endif
