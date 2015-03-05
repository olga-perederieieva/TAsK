#ifndef FILE_READER
#define FILE_READER

#include <string>
#include <iostream>
#include <fstream>

/** \brief This class simplifies the interface of using operations related to reading
	a file (like a Facade pattern).
*/
class FileReader {
	public:
		/** @param fileName path to an existing file. An error is thrown if file cannot be opened.
		*/
		FileReader(const std::string& fileName);
		~FileReader();
		
		/** This method checks if it is still possible to read information from file.
			@return false if the end of file is reached, true otherwise.
		*/
		bool isGood() const;
		
		/** This method reads a line from file and returns it.
		*/
		std::string getNextLine(); 
		
	private:
		std::ifstream file_;
		const std::string name_;
};

#endif
