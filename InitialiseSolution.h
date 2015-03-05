#ifndef _INITIALISE_SOLUTION_
#define _INITIALISE_SOLUTION_

#include "ShortestPath.h"
#include "PairOD.h"

#include <stdlib.h>
#include <cassert>

/** \brief Abstract class that defines methods to initialise different container objects used 
	in equilibration algorithms. 
*/
class InitialiseSolution {
	public:
		virtual ~InitialiseSolution(){};
		
		/** Initialises container object (set of path belonging to one O-D pair, one bush, etc.).
			@param link link of the graph.
			@param dest destination node.
			@param originIndex index of origin.
		*/
		virtual void initialiseItself(StarLink* link, PairOD *dest, int originIndex) = 0;
		
		/** This method is called in AONAssignment. It goes through all links that belong to the 
			shortest path from \b originIndex to destination \b dest and calls initialiseItself() 
			for each link. 
		*/
		void initialiseSolution(PairOD *dest, ShortestPath *shPath, int originIndex){

			StarLink *link = shPath->getInComeLink(dest->getIndex());
			assert(link != NULL);
			int nextDest = link->getNodeFromIndex();

			while (link != NULL) {
				initialiseItself(link, dest, originIndex);
				nextDest = link->getNodeFromIndex();
				link = shPath->getInComeLink(nextDest);
			}
		};
		
		
	protected:
		InitialiseSolution(){};
		
};

#endif
