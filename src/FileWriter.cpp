#include "FileWriter.h"
#include "Error.h"

FileWriter::FileWriter(const std::string& fileName, bool append) : name_(fileName) { 
	if (append) {
		file_.open(fileName.c_str(),  std::ios::out | std::ios::app);
	} else {
		file_.open(fileName.c_str(),  std::ios::out | std::ios::trunc);
	}
	if (!file_.is_open()) {
		std::string message("Cannot create file: ");
		message.append(fileName);
		throw Error(message);
	}
};

FileWriter::~FileWriter(){
	file_.close();
};

void FileWriter::writeLine(const std::string& line){
	file_ << line;
};

void FileWriter::addEmptyLine(){
	file_ << "\n";
};

std::string FileWriter::getName() const{
	return name_;
};
