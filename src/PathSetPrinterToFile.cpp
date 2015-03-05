#include "PathSetPrinterToFile.h"
#include "FileWriter.h"
#include "Path.h"
#include "ODSet.h"
#include "PathSet.h"
#include "ODMatrix.h"

#include <sstream>
#include <iomanip> 

PathSetPrinterToFile::PathSetPrinterToFile(ODMatrix* mat):
		mat_(mat) {

};

PathSetPrinterToFile::~PathSetPrinterToFile(){

};

void PathSetPrinterToFile::writePathSetToFile(PathSet* pathSet, const std::string& fileName) const{
	FileWriter writer(fileName);
	for (ConstODSetIterator odIt = pathSet->begin(); odIt != pathSet->end(); ++odIt){
		ODSet* od = *odIt;
		writer.writeLine(createStringForOD(od, od->getIndex()));
		long int pathCount = 0;
		for (PathIterator pathIt = od->begin(); pathIt != od->end(); ++pathIt) {
			writer.writeLine(createStringForPath(*pathIt, pathCount));
			++pathCount;
		}
	}
};

std::string PathSetPrinterToFile::createStringForOD(ODSet* od, long int index) const {
	int originIndex = od->getOriginIndex();
	int destIndex = od->getDestIndex();
	std::stringstream ss;
	ss << "Path set {" << index << "}: originIndex = {" << originIndex << 
		"}, destIndex = {" << destIndex << "}, demand = {" << mat_->getDemandByIndex(originIndex,
																					 destIndex)
		<< "}, nbPaths = {" << od->getNbPaths() << "}; \n";
	return ss.str(); 
};

std::string PathSetPrinterToFile::createStringForPath(Path* path, long int index) const {
	std::stringstream ss;
	ss << "Path #" << index << ": {";
	int linkIndex = -1;
	for (StarLinkIterator it = path->begin(); it != path->end(); ++it) {
		linkIndex = (*it)->getIndex();
		ss << linkIndex << " ";
	}
	ss << "}";
	ss << std::setprecision(22) << "\n" << "\t flow = {" << path->getFlow() << "}\n\t time = {" << 
		path->getCurrCost() << "}\n";
	return ss.str();
};