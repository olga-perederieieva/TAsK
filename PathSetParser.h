#ifndef PATH_SET_PARSER
#define PATH_SET_PARSER 

#include <string>

class PathSet;
class StarNetwork;

namespace PathSetParser {
	/** Parser for path sets from a text file and adds them to paths.
		@param paths PathSet object where all path sets will be added.
		@param net network.
		@param fileName path to file with path sets.
	*/
 	void parse(PathSet& paths, const StarNetwork& net, const std::string& fileName);
};

#endif