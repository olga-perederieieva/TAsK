#ifndef USED_TYPES
#define USED_TYPES

#include <vector>

/** This type defines what precision must be used for all floating point 
	calculations (double or long double). It can be changed in Makefile
	by defining USE_EXTENDED_PRECISION (long double) or not (double).
*/
#ifdef USE_EXTENDED_PRECISION	
typedef long double FPType;
#else
typedef double FPType;
#endif

typedef int TollType; /**< type for toll values.*/

typedef std::vector<TollType> TollContainerType; /**< type of container with tolls.*/

#endif
