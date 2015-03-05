#ifndef SHORTEST_PATH
#define SHORTEST_PATH

#include "StarLink.h"

#include <iostream>

/** \brief This is an abstract class that defines the interface for single-objective 
	shortest-path algorithms.
 */
class ShortestPath {
	
	public:
		virtual ~ShortestPath(){};
		
		/** Calculates shortest path from origin \b originIndex to all other nodes
			that are reachable from this origin.
		*/
		virtual void calculate(int originIndex) =0;  

		/** Calculates shortest path from origin \b originIndex to destination \b destIndex.
			If it is not over-ridden by children classes usual shortest path from a given origin
			to all other nodes is called.
		*/
		virtual void calculate(int originIndex, int destIndex, int odPairIndex) {
			calculate(originIndex);
		}; 
		
		/** @return shortest path distance to node \b destIndex */
		virtual FPType getCost(int destIndex) const =0;
		
		/** @return pointer to a link that belongs to the shortest path and
			points to node \b destIndex, return NULL if the origin is passed or
			if destIndex is not reachable from current origin.
		*/
		virtual StarLink* getInComeLink(int destIndex) const =0;  
		
		/** For debugging. Prints shortest path from origin to destIndex on screen.
		*/
		void printPath(int destIndex) const {
			StarLink *link = getInComeLink(destIndex);
			int nextDest = -1;
			std::string str;
			while (link != NULL) {
				str = link->toString() + " " + str;
				nextDest = link->getNodeFromIndex();
				link = getInComeLink(nextDest);
			}
			std::cout << str << std::endl;
		};
		
	protected:
		ShortestPath(){};

};

#endif
