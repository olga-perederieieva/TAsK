#include "TollsManagement.h"
#include "Utils.h"
#include "UtilsString.h"
#include "StarNetwork.h"
#include "Origin.h"
#include "FileWriter.h"
#include "FileReader.h"
#include "ShortestPath.h"
#include "ODMatrix.h"
#include "PairOD.h"

#include <sstream>
#include <cassert>
#include <stdlib.h>

FPType getRndNumberWithProbability(TollType val, 
		FPType probabylity, TollType maxToll){
	if (Utils::generateRndNumber(1.0) <= probabylity) {
			return rand() % maxToll + 1;
	}
	return val;
};

void assignRndTolls(ShortestPath* shPath, int destIndex, TollContainerType& tolls, 
		FPType probabylity, TollType maxToll){
	StarLink *link = shPath->getInComeLink(destIndex);
	int nextDest = link->getNodeFromIndex();
	int linkIndex = -1;
	while (link != NULL) {
		linkIndex = link->getIndex();
		tolls[linkIndex] = getRndNumberWithProbability(tolls[linkIndex], probabylity, maxToll);
		nextDest = link->getNodeFromIndex();
		link = shPath->getInComeLink(nextDest);
	}
};

bool isHeaderLine(const std::string& line){
	return line.find("<") != std::string::npos;
};

bool isCommentLine(const std::string& line) {
	return line.find("~") != std::string::npos;
};
void createHeader(FileWriter& writer, FileReader& reader) {
	std::string line("");
	while (true){
		line = reader.getNextLine() + "\n";
		if (isHeaderLine(line)) {
			writer.writeLine(line);
		} else if (isCommentLine(line)) {
			writer.writeLine(line);
			return;
		}
		if (!reader.isGood()) return;
	}
};

int findLinkIndex(int tail, int head, StarNetwork* net, int guessIndex){
	StarLink* link = net->getLink(guessIndex);
	assert(link != NULL);
	if (link->getNodeFrom() == tail && link->getNodeTo() == head) {
		return link->getIndex();
	}
	return -1;
};

void writeLinkToFile(FileWriter& writer, const std::string& line, 
	const TollContainerType& tolls, StarNetwork* net, int guessIndex) {
	std::stringstream ss(line);
	int tail = -1;
	int head = -1;
	std::string capacity("");
	std::string length("");
	std::string freeFlow("");
	std::string b("");
	std::string power("");
	std::string speed("");
	std::string toll("");
	std::string type("");
	ss >> tail >> head >> capacity >> length >> freeFlow >> b >> power >> speed >> toll >> type;
	int linkIndex = findLinkIndex(tail, head, net, guessIndex);
	assert(linkIndex != -1);
	ss.str("");
	ss << tail << "\t" << head << "\t" << capacity << "\t" << length << "\t" <<
		freeFlow << "\t" << b << "\t" << power << "\t" << speed << "\t" << tolls[linkIndex]
		<< "\t" << type << "\t;" << "\n";
	writer.writeLine(ss.str());
};



void TollsManagement::generateRandomTolls(TollContainerType& tolls, 
		FPType probabylity, TollType maxToll){
	int size = tolls.size();
	for (int i = 0; i < size; ++i) {
		tolls[i] = getRndNumberWithProbability(tolls[i], probabylity, maxToll);
	}
};

void TollsManagement::generateRandomTollsOnShortestPath(TollContainerType& tolls, 
		FPType probabylity, TollType maxToll, ODMatrix* mat, ShortestPath *shPath){
	for (OriginIterator it = mat->begin(); it != mat->end(); ++it) {
		Origin* origin = *it;
		shPath->calculate(origin->getIndex());
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
			if (Utils::generateRndNumber(1.0) <= probabylity) {
				PairOD* dest = *jt;
				assignRndTolls(shPath, dest->getIndex(), tolls, probabylity, maxToll);
			}
		}
		
	}
};

void TollsManagement::writeTollsToFile(const TollContainerType& tolls, const std::string& existingFile, 
		const std::string& newFile, StarNetwork* net){
	FileWriter writer(newFile);
	FileReader reader(existingFile);

	createHeader(writer, reader);

	std::string line("");
	int guessIndex = 0;
	while (reader.isGood()) {
		line = reader.getNextLine();
		if (!Utils::deleteWhiteSpaces(line).empty()) {
			writeLinkToFile(writer, line, tolls, net, guessIndex);
			++guessIndex;
		}
	}
	
};
