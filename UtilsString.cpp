#include "UtilsString.h"
#include "Error.h"

#include <cstring>
#include <algorithm>
#include <stdlib.h>

void Utils::checkEmptyValue(int value){
	if (value == 0) {
		std::string message = "Some values of attributes are missing";
		Error er(message);
		throw er;
	}
};

FPType Utils::extractNumber(const std::string& line, size_t pos, size_t &nextPos){
	int len = line.length();
	int i = pos;
	char ch;
	bool dotAdded = false;
	for (; i < len; ++i) {
		ch = line.at(i);
		if (isdigit(ch) || (ch == '.')) {
			if (ch == '.') dotAdded = true;
			break;
		}
	}
	if (i == len) throw Error("Current line does not contain number.");
	std::string num(1, line.at(i));
	
	int j = i + 1;
	for (; j < len; ++j) {
		ch = line.at(j);
		if (ch == '.') {
			if (dotAdded) {
				break;
			} else {
				dotAdded = true;
			}
		} else if (!isdigit(ch)) {
			break;
		}
		num.push_back(ch);
	}
	nextPos = j;
	return atof(num.c_str());
};

int Utils::extractInt(const std::string& line, size_t pos){
	int len = line.length();
	int i = pos;
	for (; i < len; ++i) {
		if (isdigit(line.at(i))) break;
	}
	if (i == len) throw Error("Current line does not contain integers.");
	std::string num(1, line.at(i));
	char ch;
	for (int j = i + 1; j < len; ++j) {
		ch = line.at(j);
		if (!isdigit(ch)) break;
		num.push_back(ch);
	}
	return atoi(num.c_str());
};
		


std::string Utils::deleteWhiteSpaces(const std::string &line){
	std::string lineWithoutSpaces(line);
	lineWithoutSpaces.erase(std::remove_if(lineWithoutSpaces.begin(), lineWithoutSpaces.end(), isspace), 
		lineWithoutSpaces.end());
	return lineWithoutSpaces;
}

std::string Utils::getSubString(const std::string& first, const std::string& last, const std::string& line){
	size_t found = line.find(first);
	if (found == std::string::npos) {
		std::string message = first; 
		message.append(" is missing in line: ");
		message.append(line);
		Error er(message);
		throw er;	
	}
	size_t foundEnd = line.find(last);
	if (foundEnd == std::string::npos) {
		std::string message = last; ;
		message.append(" is missing in line: ");
		message.append(line);
		Error er(message);
		throw er;
	}
	if (found > foundEnd) {
		std::string message = "Field name or value is missing in line: ";
		message.append(line);
		Error er(message);
		throw er;
	}
	if (foundEnd - found <= 1) return "";
	return line.substr(found + 1, foundEnd - found - 1);
};

std::string Utils::skipOneLineComment(const std::string& comment, const std::string& line){
	size_t found = line.find(comment);
	if (found != std::string::npos) {
		if (found == 0) {
			return "";
		} else {
			return line.substr(0, found - 1);
		}
	}
	return line;
};

void Utils::extractName(const char* filepath, std::string& name){
	
	// Format of file path: "/bla/bla/bla/problemName.someextension"
	// We try to find the interval [begin; end) which contains "problemName"
	const char* end = filepath + strlen(filepath);

	// We rewind "end"
	// Find the position of the last point (filepath if there is no such point)
	while(end > filepath) {
		if(*(end-1) == '.'){
			--end;
			break;
		}
		--end;
	}

	const char* begin = end;

	while(      begin  > filepath
        && *(begin-1) != '/'
		&& *(begin-1) != '\\'){
		--begin;
	}

	name.assign(begin, end);
};

bool Utils::isDigits(const std::string &str){
	bool hasPoint = false;
	int length = str.length();
	for (int i = 0; i < length; ++i) {
		if (!isdigit(str.at(i))) {
			if (str.at(i) == '.' && !hasPoint && i != length-1) {
				hasPoint = true; 
			} else {
				return false;
			}
		}	
	}
	return true;
   
};

bool Utils::isInt(const std::string &str){
	return str.find_first_not_of("0123456789") == std::string::npos;
};
