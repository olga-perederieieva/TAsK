#ifndef FILE_WRITER
#define FILE_WRITER

#include <fstream>
#include <string>

/** \brief This class simplifies the interface of using operations related to writing
	information to a file (like a Facade pattern).
*/
class FileWriter {
	public:
		/** @param fileName path to a file. If the file did not exist it will be created.
			@param append (default value is false) must specify if the file should be appended (true) 
			or over-written (false). 
		*/
		explicit FileWriter(const std::string& fileName, bool append = false);
		~FileWriter();
		
		/** Writes a line to the file. 
			\note It does NOT add end of line symbol to the line.
		*/
		void writeLine(const std::string& line);
		/** Adds an empty line to the file.
		*/
		void addEmptyLine();
		
		/** @return file name.
		*/
		std::string getName() const;
		
	private:
		const std::string name_;
		std::ofstream file_;
};

#endif

