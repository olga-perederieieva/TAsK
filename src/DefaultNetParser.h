#ifndef DEFAULT_NET_PARSER
#define DEFAULT_NET_PARSER 

#include "UsedTypes.h"

#include <string>

class StarNetwork;
class FileReader;

/** Internal utility structure.
*/
struct DataFromHeader { 
	int nbNodes;
	int nbLinks; 
	int firstNode;
};

/** \brief Implements parser for network from text file.
	
	File format is described <a href="http://www.bgu.ac.il/~bargera/tntp/"> here </a>.
*/
class DefaultNetParser {
	public:
		/** @param fileWithNetwork path to file with network.
		*/
		DefaultNetParser(const std::string& fileWithNetwork);
		virtual ~DefaultNetParser();

		/** Parses network from file specified in the class constructor 
			and creates a StarNetwork object. 
			\note  It is assumed that all links are SORTED according to ascending order of head nodes
		*/
		virtual StarNetwork* parseNetwork();

	protected:
		const std::string fileWithNetwork_;

		/** Hook method for derived classes for the case when file with network
			also contains link tolls.
		*/
		virtual void allocateTolls(int nbLinks) {};
		/** Hook method for derived classes for the case when file with network
			also contains link tolls.
		*/
		virtual void setToll(int linkIndex, TollType toll) {};

	private:
		
		DataFromHeader parseDataFromHeader(FileReader& readFile);

};
#endif