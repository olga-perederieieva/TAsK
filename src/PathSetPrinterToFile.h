#ifndef PATH_SET_PRINTER_TO_FILE
#define PATH_SET_PRINTER_TO_FILE

#include <string>

class ODMatrix;
class PathSet;
class ODSet;
class Path;

/** \brief Prints path sets to text file.
*/
class PathSetPrinterToFile {
	public:
		/** @param mat O-D matrix.
		*/
		PathSetPrinterToFile(ODMatrix* mat);
		~PathSetPrinterToFile();

		/** Writes path sets to file.
			@param pathSet pointer to the PathSet object.
			@param fileName path to file where path sets will be written.
		*/
		void writePathSetToFile(PathSet* pathSet, const std::string& fileName) const;

	private:
		ODMatrix* mat_;

		std::string createStringForOD(ODSet* od, long int index) const;
		std::string createStringForPath(Path* path, long int index) const;
	
};
#endif