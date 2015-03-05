#ifndef UTILS_STRING
#define UTILS_STRING

#include "UsedTypes.h"

#include <string>

/** \brief Defines useful function for operations with strings.
*/
namespace Utils {
	
	/** Throws an error if value is zero.
		*/
	void checkEmptyValue(int value);
	/** Given line, extracts an integer value starting from position pos.
		Throws an error if line doesn't contain a number.
		@return extracted integer value.
	*/
	int extractInt(const std::string& line, size_t pos);
	/** Given line, extracts a floating point value starting from position pos.
		Writes end position of extracted floating point value to nextPos.
		Throws an error if line doesn't contain a number.
		@return extracted floating point value.
	*/
	FPType extractNumber(const std::string& line, size_t pos, size_t &nextPos);

	/** @return the first sub-string of string \b line that is between 
		\b first and \b last strings. For example
		@code getSubString("<",">","{example}:<value>");
		@endcode
		will return \b value.
		\note \b first and \b last are assumed to be different strings, otherwise 
		method will return an empty string.
	*/
	std::string getSubString(const std::string& first, const std::string& last, 
		const std::string& line);

	/** Format of file path \b filePath : "/bla/bla/bla/problemName.someextension"
		@return \b problemName
		@author Sylvain Rosembly
	*/
	void extractName(const char* filepath, std::string& name);

	/** Extracts one line comment from a string. For example:
		@code skipOneLineComment("//", "i = 10*j + 4; //some comments "); @endcode
		will return i = 10*j + 4;
	*/
	std::string skipOneLineComment(const std::string& comment, const std::string& line);

	/** Takes a string \b line and returns a new string without white spaces from \b line.
	*/
	std::string deleteWhiteSpaces(const std::string &line);

	/** @return true if \b str is a digit (possible non-integral), false otherwise.
		\note Only strings of this format *.* (or *) where * is an integral number 
		are recognised as a digit (1E-10 is not considered a number).
	*/
	bool isDigits(const std::string &str);
	
	/** @return true if \b str is an integer, false otherwise.
	*/
	bool isInt(const std::string &str);

}

#endif
