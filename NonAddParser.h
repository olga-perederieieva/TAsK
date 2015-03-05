#ifndef NON_ADD_PARSER
#define NON_ADD_PARSER

#include "DefaultNetParser.h"

/** \brief Parser for the network specified in the text file when tolls
	are described in the same file as the network itself. 

	For file format, see <a href="http://www.bgu.ac.il/~bargera/tntp/"> here </a>.
*/
class NonAddParser : public DefaultNetParser {
	public:
		NonAddParser(const std::string& fileWithNetwork);
		~NonAddParser();

		/** @return pointer to the allocated array of link tolls.
		*/
		TollContainerType* getTolls();

	protected:
		/** Allocates memory for the array with link tolls.
		*/
		virtual void allocateTolls(int nbLinks);
		/** Sets toll \b toll for the link with index linkIndex.
		*/
		virtual void setToll(int linkIndex, TollType toll);	

	private:
		TollContainerType* tolls_;

};
#endif