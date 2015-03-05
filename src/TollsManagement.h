#ifndef TOLLS_MANAGEMENT
#define TOLLS_MANAGEMENT 

#include "UsedTypes.h"

#include <string>

class ShortestPath;
class ODMatrix;
class StarNetwork;

/** \brief Defines utility functions for operation with tolls.
*/
namespace TollsManagement {

	/** Generates random tolls in interval [1, maxToll] with probability \b probability
		and writes all tolls to container tolls.
	*/
	void generateRandomTolls(TollContainerType& tolls, FPType probabylity, TollType maxToll);
	/** Generates random tolls on shortest paths in interval [1, maxToll]
		with probability \b probability
		and writes all tolls to container tolls.
	*/
	void generateRandomTollsOnShortestPath(TollContainerType& tolls, 
		FPType probabylity, TollType maxToll, ODMatrix* mat, ShortestPath *shPath);
	/** Writes tolls to file newFile based on existing file existingFile that describes
		network.
	*/
	void writeTollsToFile(const TollContainerType& tolls, const std::string& existingFile, 
		const std::string& newFile, StarNetwork* net);
};

#endif