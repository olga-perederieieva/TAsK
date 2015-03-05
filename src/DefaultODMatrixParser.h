#ifndef DEFAULT_OD_MATRIX_PARSER
#define DEFAULT_OD_MATRIX_PARSER 

#include "UsedTypes.h"

#include <string>

class ODMatrix;
class StarNetwork;

/** \brief Implements parser for O-D matrix from text file.
	
	File format is described <a href="http://www.bgu.ac.il/~bargera/tntp/"> here </a>.
*/
class DefaultODMatrixParser {
	public:
		/** @param fileWithODMatrix path to file with O-D matrix.
		*/
		DefaultODMatrixParser(const std::string& fileWithODMatrix);
		~DefaultODMatrixParser();

		/** Parses O-D matrix from file specified in class constructor
			and creates an ODMatrix object. 
			\note After object is created setIndexes() method is called,
			so there is no need to call it again. 
		*/
		virtual ODMatrix* parseODMatrix(StarNetwork* net);

	protected:
		const std::string fileWithODMatrix_;

		/** Extracts destination from line starting from position posFrom,
			writes its ID to destID and demand to demand.
			@return position in line to start looking for the next destination,
				std::string::npos if no destination is found.
		*/
		virtual size_t extractDestination(size_t posFrom, const std::string &line, 
			int &destID, FPType &demand) const;
     
};

#endif