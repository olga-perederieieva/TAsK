#ifndef PATH_AND_DIRECTION
#define PATH_AND_DIRECTION

#include "UsedTypes.h"

class Path;

/** \brief This class is a basic element used in DescDirectionPath. It represents a pair 
	of two values: Path and direction value associated with a given Path.
*/
class PathAndDirection {
	public:
		PathAndDirection();
		~PathAndDirection();
		
		/** @return pointer to object that stores path.
		*/
		Path* getPath() const;
		/** @return value of the direction of descent.
		*/
		FPType getDirectionValue() const;
		
		/** Sets pointer of the object that stores path.
		*/
		void setPath(Path *path);
		/** Sets direction of descent value.
		*/
		void setDirection(FPType direction);
		
	private:
		Path* path_;
		FPType directionVal_;
};

#endif
