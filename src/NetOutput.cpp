#include "NetOutput.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "UtilsString.h"
#include "Error.h"
#include "StarNetwork.h"

#include <sstream>

NetOutput::NetOutput(StarNetwork* net, const std::string& logFileForMissingIDs) : net_(net),
			logFileForMissingIDs_(logFileForMissingIDs) {

};

NetOutput::~NetOutput(){

};

void NetOutput::readCoord(const std::string& file, std::vector<FPType>& xCoord, 
				std::vector<FPType>& yCoord, std::vector<int>& nodeID){
	FileReader fileCoords(file);
	std::string temp = fileCoords.getNextLine(); // skip first line
	bool someIDMissing = false;
	FileWriter writeMissingIDs(logFileForMissingIDs_);
	while (fileCoords.isGood()){
		std::string line = fileCoords.getNextLine();
		if (!Utils::deleteWhiteSpaces(line).empty()) {
			int nodeIDval;
			FPType x;
			FPType y;
			std::stringstream lineToTokenize(line);
			lineToTokenize >> nodeIDval >> x >> y >> temp;
			try {
				int nodeIndex = net_->getNodeIndex(nodeIDval);
				nodeID[nodeIndex] = nodeIDval;
				xCoord[nodeIndex] = x; 
				yCoord[nodeIndex] = y; 		
			} catch (Error e) {
				someIDMissing = true;
				writeMissingIDs.writeLine(e.getMessage() + "\n");
			}
		}
	}
	if (someIDMissing) {
		std::cout << "Some node IDs are missing. See " << logFileForMissingIDs_ << std::endl;
	}
};

