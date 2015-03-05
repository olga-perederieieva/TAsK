#ifndef UTILS_
#define UTILS_

#include "UsedTypes.h"

#include <list>
#include <string>

class Path;
class PathBasedFlowMove;
class StarNetwork;
class ODMatrix;
class SpiessFncCreator;
class PathSet;
class StarLink;

struct Coords {
	int nodeID;
	int x;
	int y;
} ;

/** \brief This namespace defines various useful functions.
*/
namespace Utils {

	/** Finds all disjoint links of paths \b path1 and \b path2 and writes them into
		list \b list.
	*/
	void getDisjointLinks(Path *path1, Path *path2, std::list<StarLink*> &list, int nbLinks);

	/** Finds all common links of paths specified in \b paths and writes them into
		list \b list.
	*/
	void getCommonLinks(PathBasedFlowMove *paths, std::list<StarLink*> &list, int nbLinks);

	/** Checks if network flow constraints are satisfied at each node of the 
		graph.
		@return maximum link flow deviation.
	*/
	FPType checkFeasibility(StarNetwork *net, ODMatrix *mat);

	/** Parses string \b line and returns Spiess link cost functions parameters into 
		SpiessFncCreator \b spCreator.
		@return id of generated Spiess function.
	*/
	int parseSpiessFnc(const std::string& input, SpiessFncCreator &spCreator, bool constFnc = false);
	
	/** Calculates derivative w.r.t. flow shift between two paths \b path 
		and \b minPath.
	*/
	FPType calculatePathDerivative(Path *path, Path *minPath, int nbLinks);
	
	/** Same as calculatePathDerivative() but disjoint links are already known and stored 
		in list \b list.
	*/
	FPType calculatePathDerivativeForDisjoint(const std::list<StarLink*> &list);
	
	/** Same as calculatePathDerivative() but disjoint links are already known and stored 
		in two lists \b list1 and \b list2.
	*/
	FPType calculatePathDerivativeForDisjoint(const std::list<StarLink*> &list1,
											  const std::list<StarLink*> &list2);

	/** Reads coordinates from file art3NodesFile and writes them into list newCoords.
	*/
	void readART3Coordinates(const std::string& art3NodesFile, std::list<Coords>& newCoords);

	/** Copies coordinates from coords to newCoords for nodes that are present in net.
	*/
	void mapCoordsFromART3(const std::list<Coords>& coords, StarNetwork* net,
						   std::list<Coords>& newCoords);
	/** Writes coordinates to file fileName.
	*/
	void writeCoordsToFile(const std::list<Coords>& coords, const std::string& fileName);
	/** Very simple implementation to tokenize string. It relies on stringstream ability to
		tokenize string.
	*/
	void tokenizeIntoStr(const std::string& lineToTokenize, std::string* result, int nbTokens);
	/** @return Coords object with filled attributes if nodeID is present in coords,
			if not present all attributes are set to -1.
	*/
	Coords findCoords(const std::list<Coords>& coords, int nodeID);

	/** @return random floating point number in interval [0, maxVal].
	*/
	FPType generateRndNumber(FPType maxVal);

}

#endif
