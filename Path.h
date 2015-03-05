#ifndef PATH_
#define PATH_

#include "UsedTypes.h"

#include <list>

class StarLink;

typedef std::list<StarLink*>::iterator StarLinkIterator;
typedef std::list<StarLink*>::const_iterator ConstStarLinkIterator;

/** \brief This class represents a path.
*/
class Path {
	public:
		Path(); 
		~Path();
		
		/** Sets flow value.
		*/
		void setFlow(FPType flow);
		/** Stores previous value of path flow. This is needed for path-based algorithms.
		*/
		void setPrevFlow(FPType prevFlow); 
		/** Sets current path cost.
		*/
		void setCurrCost(FPType cost);
		
		/** Adds link in front of a list that stores all links.
		*/
		void addLinkToPath(StarLink *link);
		/** Adds link in the end of a list that stores all links. 
		*/
		void pushBackLink(StarLink* link);
		
		/** @return path flow.
		*/
		FPType getFlow() const;
		/** @return previous path flow.
		*/
		FPType getPrevFlow() const;
		/** @return current path cost.
		*/
		FPType getCurrCost() const;
		/** Calculates current path travel time.
			@return calculated current path travel time.
		*/
		FPType calcCurrTime() const;
		/** @return iterator pointing to the beginning of the container
				with links belonging to this path.
		*/
		StarLinkIterator begin();
		/** @return end of container.
		*/
		StarLinkIterator end();
		/** @return const iterator pointing to the beginning of the container
				with links belonging to this path.
		*/
		ConstStarLinkIterator begin() const;
		/** @return end of const container.
		*/
		ConstStarLinkIterator end() const;
		/** @return true if path doesn't have any links, false otherwise.
		*/
		bool isEmpty() const;
		/** @return number of links that belong to this path.
		*/
		int size() const;
		/** Prints path on screen.
		*/
		void print();
		/** Removes all links from the path.
			\note memory occupied by each link object is not deallocated in this method.
		*/
		void clear();
		
	private:
		std::list<StarLink*> links_;
		FPType flow_;
		FPType prevFlow_;
		FPType currCost_;
		
};

#endif
