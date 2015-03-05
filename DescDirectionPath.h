
#ifndef DESC_DIRECTION_PATH
#define DESC_DIRECTION_PATH

#include "PathAndDirection.h"
#include "Path.h"

#include <stdlib.h>
#include <iostream>
#include <list>
#include <limits>
#include <cassert>

/** \brief This class is a base class for different directions of descent implemented in path-based
	algorithms. 
	\details Direction of descent is in the space of path flows. 
	It is expected that only one instance of derived classes will be allocated and used.
	DescDirectionPathScaled is a friend class because it calls createPathDirection() on the pointer
	passed to it.
*/
class DescDirectionPath {
	friend class DescDirectionPathScaled;
	public:
		virtual ~DescDirectionPath(){
			for (int i = 0; i < size_; ++i) {
				delete pathDirection_[i];
			}
			delete[] pathDirection_;
		};
		/** Deallocates memory used by a previously calculated direction of descent 
			(represented by class PathAndDirection), creates a new one and returns it.
			@param size after calling this function it will contain the size of the allocated 
			array with path-based direction of descent.
			@param paths  the list of paths currently stored in a given O-D set.
			@param isEquilibrated after calling this function it will contain true if the current
			O-D set	is equilibrated (the notion of being equilibrated depends on the algorithm,
			but basically it means that it is either useless or impossible to shift flows 
			within given O-D set), false otherwise.
		*/
		PathAndDirection** calculatePathDirection(int &size, const std::list<Path*> &paths,
						bool &isEquilibrated){
			for (int i = 0; i < size_; ++i) {
				delete pathDirection_[i];
			}
			delete[] pathDirection_;
			pathDirection_ = createPathDirection(size_, paths, isEquilibrated);
			size = size_;
			return pathDirection_;
		};
		
	protected:
		
		const FPType delta_; /**< Stores the precision of direction of descent. */

		DescDirectionPath(FPType delta) :  delta_(delta), pathDirection_(NULL), size_(0) {};
		
		/** Finds the path with minimal cost within the set of given paths.*/
		Path* findMinPath(const std::list<Path*> &paths) const{
			FPType minDist = std::numeric_limits<FPType>::infinity( );
		  	Path *minPath = NULL;
		  	FPType cost = 0.0;
		  	Path* path = NULL;
		  	for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
		  		path = *it;
		  		cost = path->getCurrCost();
		  		if (cost < minDist) {
		  			minDist = cost;
		  			minPath = path;
		  		}
		  	}
		  	assert(minPath != NULL);
		  	return minPath;
		};
		
		/** @return path with maximal cost within the set of given paths. 
			It also ensures that it does not coincide with the path with minimal cost.
		*/
		Path* findMaxPath(const std::list<Path*> &paths, Path *minPath) const{
			Path *maxPath = NULL;
		  	FPType maxDist = 0.0;
		  	FPType cost = 0.0;
		  	Path* path = NULL;
		  	for (std::list<Path*>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
		  		path = *it;
		  		if (path != minPath) {
		  			cost = path->getCurrCost();
		  			if (cost > maxDist) {
			  			maxDist = cost;
			  			maxPath = path;
		  			}
		  		}
		  	}
		  	assert(maxPath != NULL);
		  	return maxPath;
		};
		
		/** This method is responsible for creation of direction of descent. It depends on a
			particular path-based algorithm.
		*/
		virtual PathAndDirection** createPathDirection(int &size, const std::list<Path*> &paths, 
						bool &isEquilibrated) = 0;
		
	private:
		PathAndDirection** pathDirection_;
		int size_;
};

#endif
