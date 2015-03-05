#include "ParseParams.h"
#include "Error.h"
#include "UtilsString.h"

#include <fstream>

ParseParams::ParseParams() : longComment_(false){

};

ParseParams::~ParseParams(){

};

void ParseParams::execute(const std::string &fileName, Params *params){
	// open file
	longComment_ = false;
	std::ifstream paramsFile(fileName.c_str());
	if (paramsFile.is_open()) {
		
		// parse all fields
		std::string line = "";
		std::string field = "";
		std::string value = "";
		while ( paramsFile.good() ) {
			getline(paramsFile, line);
			line = skipComments(line);
			if (!Utils::deleteWhiteSpaces(line).empty()){
				field = Utils::getSubString("<", ">", line); // First entry of form <FIELD NAME> 
															 // will be treated.
				value = Utils::getSubString("{", "}", line); // First entry of form {VALUE} 
															 // will be treated.
				params->addParameter(field, value);
			}
		}
		
		// close file
		paramsFile.close();
	} else {
		std::string message = "Unable to open a file: ";
		message.append(fileName);
		Error er(message);
		throw er;
	}
	
};

std::string ParseParams::skipComments(std::string line){
	size_t found;
	if (longComment_) {
		found = line.find("*/");
		if (found != std::string::npos) {
			longComment_ = false;
			return line.substr(found+2);
		}
		return "";	
	}
	found = line.find("//");
	std::string resLine = line;
	if (found != std::string::npos) {
		resLine = line.substr(0, found);
	}
	found = resLine.find("/*");
	if (found != std::string::npos) {
		longComment_ = true;
		resLine = resLine.substr(0, found);
	}
	return resLine;
};
