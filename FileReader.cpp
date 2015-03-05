#include "FileReader.h"
#include "Error.h"

#include <iostream>

FileReader::FileReader(const std::string& fileName) : file_(fileName.c_str()), name_(fileName){
	if (!file_.is_open()) {
		std::string message = "Unable to open a file: ";
		message.append(name_);
		Error er(message);
		throw er;
	}
};

FileReader::~FileReader(){
	file_.close();
};
		
bool FileReader::isGood() const{
	return file_.good();
};

std::string FileReader::getNextLine(){
	std::string line;
	getline(file_, line);
	return line;
};

