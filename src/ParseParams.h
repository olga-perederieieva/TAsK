#ifndef PARSE_PARAMS
#define PARSE_PARAMS

#include "Params.h"

#include <string>

/** \brief This class is a parser for file with parameters.
*/
class ParseParams {
	public:
		explicit ParseParams();
		~ParseParams();
		
		/** Adds attributes specified in file \b fileName to an object \b params given as input.
			This method might throw an error. 
		*/
		void execute(const std::string &fileName, Params *params);
		
	private:
		bool longComment_;
		
		/** @return string without comments if there were any.
			\note Supported comments: \b/\b*, \b/\b/
		*/
		std::string skipComments(std::string line);
		
};

#endif
