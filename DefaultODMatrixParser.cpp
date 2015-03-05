#include "DefaultODMatrixParser.h"
#include "Error.h"
#include "FileReader.h"
#include "UtilsString.h"
#include "Origin.h"
#include "PairOD.h"
#include "ODMatrix.h"
#include "StarNetwork.h"

#include <cassert>
#include <stdlib.h>

DefaultODMatrixParser::DefaultODMatrixParser(const std::string& fileWithODMatrix) : 
		fileWithODMatrix_(fileWithODMatrix) {

};

DefaultODMatrixParser::~DefaultODMatrixParser(){

};

ODMatrix* DefaultODMatrixParser::parseODMatrix(StarNetwork* net){
	FileReader readFile(fileWithODMatrix_);
	int nbNodes = net->getNbNodes();
	// skip metadata
	std::string line = "";
	while (line.find("<END OF METADATA>") == std::string::npos) {
		line = readFile.getNextLine(); 
		
		if (!readFile.isGood()){ 
			std::string message = "<END OF METADATA> is missing in file: ";
			message.append(fileWithODMatrix_);
			Error er(message);
			throw er;
		}
	}
	size_t found;
	int originID = -1;
	int destID = -1;
	FPType demand = 0.0;
	Origin *origin = NULL;
	std::list<Origin*> allOrigins;
	std::string newLine = "";
	while (readFile.isGood()) { 
		line = readFile.getNextLine();
		line = Utils::skipOneLineComment("~", line);
		if (!Utils::deleteWhiteSpaces(line).empty()){
			// check if it is a new origin
			found = line.find("Origin");
			if (found != std::string::npos) {
				originID = atoi((line.substr(found + 7)).c_str());
				origin = new Origin(net->getNodeIndex(originID));
				allOrigins.push_back(origin);	
			}
			assert(origin != NULL);
			// get all destinations from current line
			size_t pos = extractDestination(0, line, destID, demand);
			
			while (pos != std::string::npos){
				if ((destID != originID) && (demand != 0.0)) {
					origin->addDestination(new PairOD(net->getNodeIndex(destID),
											demand));
				}
				pos = extractDestination(pos, line, destID, demand);
			}
		}
	}
	ODMatrix *mat = new ODMatrix(nbNodes, allOrigins.size());
	for (std::list<Origin*>::iterator it = allOrigins.begin();
			it != allOrigins.end(); ++it) {
		mat->addOrigin(*it);
	}
	mat->setIndexes();
	return mat;
};

size_t DefaultODMatrixParser::extractDestination(size_t posFrom, const std::string& line, 
			int &destID, FPType &demand) const{
	size_t pos1 = line.find(":", posFrom);
	if (pos1 != std::string::npos) {
		size_t pos2 = line.find(";", pos1);
		if (pos2 != std::string::npos) {
			destID = atoi((line.substr(posFrom, pos1 - posFrom - 1)).c_str());
			demand = atof((line.substr(pos1 + 1, pos2 - pos1 - 1)).c_str());
			return pos2 + 1;
		}
		return std::string::npos;
	}
	return std::string::npos;
};