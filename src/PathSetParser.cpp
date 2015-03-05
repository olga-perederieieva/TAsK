#include "PathSetParser.h"
#include "FileReader.h"
#include "UtilsString.h"
#include "PathSet.h"
#include "StarNetwork.h"
#include "Path.h"
#include "ODSet.h"

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

struct ODPairInfo {
	int index;
	int originIndex;
	int destIndex;
	int nbPaths;
};

void printODPairInfo(const ODPairInfo& info) {
	std::cout << "index = " << info.index << " originIndex = " << info.originIndex <<
		" destIndex = " << info.destIndex << " nbPaths = " << info.nbPaths << std::endl;
};

void initialiseODPairInfo(ODPairInfo& info) {
	info.index = -1;
	info.originIndex = -1;
	info.destIndex = -1;
	info.nbPaths = -1;
};

std::string getNextIndex(const std::string& line, const std::string& indexName = "") {
	size_t indexPos = 0;
	if (indexName != "") indexPos = line.find(indexName);
	assert(indexPos != std::string::npos);
	std::string subLine = line.substr(indexPos);
	std::string index = Utils::getSubString("{", "}", subLine);
	assert(index != "");
	return index;
};

ODPairInfo parseHeader(const std::string& line) {
	ODPairInfo info;
	initialiseODPairInfo(info);
	info.index = atoi(getNextIndex(line, "Path set").c_str());
	info.originIndex = atoi(getNextIndex(line, "originIndex").c_str());
	info.destIndex = atoi(getNextIndex(line, "destIndex").c_str());
	info.nbPaths = atoi(getNextIndex(line, "nbPaths").c_str());
	return info;
};

Path* createPath(const std::string& line, const StarNetwork& net) {
	std::string links = Utils::getSubString("{", "}", line);
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(" "); 
    tokenizer tokens(links, sep);

    Path* path = new Path;
    for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter){
    	std::string token = *tok_iter;
    	path->pushBackLink(net.getLink(atoi(token.c_str())));
    }
    return path;
};

FPType getFloatValue(const std::string& line) {
	return strtold(getNextIndex(line).c_str(), NULL);
};

void PathSetParser::parse(PathSet& paths, const StarNetwork& net, const std::string& fileName) {
	
	FileReader reader(fileName);
	ODPairInfo info;
	initialiseODPairInfo(info);
	Path* path = NULL;
	while (reader.isGood()) {
		std::string line = reader.getNextLine();
		if (line.find("Path set") != std::string::npos) {
			info = parseHeader(line);
			path = NULL;
		} else if (line.find("Path") != std::string::npos) {
			assert(info.index != -1);
			ODSet* odset = paths.getODSetByIndex(info.index);
			assert(odset->getOriginIndex() == info.originIndex &&
				odset->getDestIndex() == info.destIndex);
			path = createPath(line, net);
			odset->addPath(path);
		} else if (line.find("flow") != std::string::npos) {
			assert(path != NULL);
			path->setFlow(getFloatValue(line));
		} else if (line.find("time") != std::string::npos) {
			assert(path != NULL);
			path->setCurrCost(getFloatValue(line));
		} 
	}
};