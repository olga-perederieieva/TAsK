#include "DefaultNetParser.h"
#include "UtilsString.h"
#include "Error.h"
#include "BprFnc.h"
#include "FileReader.h"
#include "StarNetwork.h"
#include "StarNode.h"
#include "StarLink.h"

#include <sstream>
#include <stdlib.h>

/** Internal utility structure.
*/ 
struct ParamsLine{
	int origin;
	int dest;
	FPType capacity;
	FPType length;
	FPType freeFlowTime;
	FPType b;
	FPType power;
	FPType speed;
	FPType toll;
	int type;
};

DefaultNetParser::DefaultNetParser(const std::string& fileWithNetwork) : 
				fileWithNetwork_(fileWithNetwork) {

};

DefaultNetParser::~DefaultNetParser(){

};

StarNetwork* DefaultNetParser::parseNetwork(){
	FileReader readFile(fileWithNetwork_);
		
	std::string netName("");
	Utils::extractName(fileWithNetwork_.c_str(), netName);
	
	DataFromHeader data = parseDataFromHeader(readFile);
	
	StarNetwork* net = new StarNetwork(data.nbNodes, data.nbLinks, netName);
	allocateTolls(data.nbLinks);

	char semic;
	ParamsLine params;
	int originPrev = -1;
	LinkFnc *fnc = NULL;
	bool isZone = false;
	int nbZones = 0;
	StarLink* link = NULL;

	std::string line("");

	while (readFile.isGood()) { 
		line = readFile.getNextLine(); 
		line = Utils::skipOneLineComment("~", line);
		
		if (!Utils::deleteWhiteSpaces(line).empty()){
    		std::stringstream strLine(line);
			strLine >> params.origin >> params.dest >> params.capacity >> params.length >> 
						params.freeFlowTime >> params.b >> params.power >> params.speed >> 
						params.toll >> params.type >> semic;
			if (originPrev != params.origin) {
				
				if (params.origin < data.firstNode) {
					isZone = true;
					++nbZones;
				} else {
					isZone = false;
				}
				net->addNode(new StarNode(params.origin, isZone));
				originPrev = params.origin;
			}
			
			fnc = new BprFnc(params.freeFlowTime, params.b, params.capacity, params.power);
			link = new StarLink(params.origin, params.dest, fnc);
			net->addLink(link);
			setToll(link->getIndex(), params.toll);
		}
	}
	return net;	 
};

DataFromHeader DefaultNetParser::parseDataFromHeader(FileReader& readFile) {
	std::string line("");
	std::string field("");
	std::string value("");
	DataFromHeader data;
	data.nbNodes = 0;
	data.nbLinks = 0;
	data.firstNode = 0;
	while (line.find("<END OF METADATA>") == std::string::npos) {
		line = readFile.getNextLine();
		line = Utils::skipOneLineComment("~", line);
		
		if (!Utils::deleteWhiteSpaces(line).empty()){
			field = Utils::getSubString("<", ">", line);
			value = line.substr(line.find(">") + 1);
			if (field == "NUMBER OF NODES"){
				data.nbNodes = atoi(value.c_str());
			} else if (field == "NUMBER OF LINKS") { 
				data.nbLinks = atoi(value.c_str());
			} else if (field == "FIRST THRU NODE") {
				data.firstNode = atoi(value.c_str());
			}
		}
		if (!readFile.isGood()) {
			std::string message = "<END OF METADATA> is missing in file: ";
			message.append(fileWithNetwork_);
			Error er(message);
			throw er;
		}
	}
	Utils::checkEmptyValue(data.nbNodes);
	Utils::checkEmptyValue(data.nbLinks);
	Utils::checkEmptyValue(data.firstNode); 
	return data;	
};