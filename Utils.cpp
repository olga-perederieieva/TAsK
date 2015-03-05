#include "Utils.h"
#include "UtilsString.h"
#include "Error.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "Origin.h"
#include "PathAndDirection.h"
#include "ODSet.h"
#include "Path.h"
#include "StarLink.h"
#include "StarNetwork.h"
#include "PathBasedFlowMove.h"
#include "ODMatrix.h"
#include "PairOD.h"
#include "PathSet.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <cassert>
#include <math.h>

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

void Utils::getDisjointLinks(Path *path1, Path *path2, std::list<StarLink*> &list, int nbLinks){
	int indexes[nbLinks];
	for (int i = 0; i < nbLinks; ++i) {
		indexes[i] = 0;
	}
	
	for (StarLinkIterator it = path1->begin(); it != path1->end(); ++it) {
		indexes[(*it)->getIndex()] = 1;
	}
	int index = -1;
	for (StarLinkIterator it = path2->begin(); it != path2->end(); ++it) {
		StarLink* link = *it;
		index = link->getIndex();
		if (indexes[index] == 0) {
			list.push_back(link);
		} else {
			indexes[index] = -1;
		}
	}
	for (StarLinkIterator it = path1->begin(); it != path1->end(); ++it) {
		StarLink* link = *it;
		if (indexes[link->getIndex()] == 1) {
			list.push_back(link);
		}
	}
	assert(!list.empty());
};

void Utils::getCommonLinks(PathBasedFlowMove *paths, std::list<StarLink*> &list, int nbLinks){
	int indexes[nbLinks];
	int indexesTmp[nbLinks];
	StarLink* links[nbLinks];
	int size = 0;
	for (int i = 0; i < nbLinks; ++i) {
		indexes[i] = 0;
		indexesTmp[i] = 0;
		links[i] = NULL;
	}
	int index = -1;
	Path *path = NULL;
	int nbPaths = 0;
	for (PathAndDirection *pathDir = paths->beginPathDirection(); pathDir != NULL; 
			pathDir = paths->getNextPathDirection()) {
		path = pathDir->getPath();
		++nbPaths;
		for(StarLinkIterator it = path->begin(); it != path->end(); ++it){
			StarLink* link = *it;
			index = link->getIndex();
			if (indexes[index] == 0) {
				indexesTmp[size] = index;
				++size;
				links[index] = link;
			}
			++indexes[index];
		}
	}
	for (int i = 0; i < size; ++i) {
		index = indexesTmp[i];
		if (indexes[index] == nbPaths) {
			list.push_back(links[index]);
		}
	}
};

FPType Utils::checkFeasibility(StarNetwork *net, ODMatrix *mat){
	FPType total[net->getNbNodes()];
	for (int i = 0; i < net->getNbNodes(); ++i) {
		total[i] = 0.0;
	}
	// load demands
	for (OriginIterator it = mat->begin(); it != mat->end(); ++it){
		Origin* origin = *it;
		for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
			PairOD* dest = *jt;
			total[origin->getIndex()] += dest->getDemand();
			total[dest->getIndex()] -= dest->getDemand();
		}
	}
	
	//travers network and check
	for (StarLink *link = net->beginOnlyLink(); link != NULL; link = net->getNextOnlyLink()) {
		total[link->getNodeFromIndex()] -= link->getFlow();
		total[link->getNodeToIndex()] += link->getFlow();
	}
	FPType maxVal = 0.0;
	for (int i = 0; i < net->getNbNodes(); ++i) {
		if (fabs(total[i]) > maxVal) maxVal = fabs(total[i]);
	}
	return maxVal;
};

FPType Utils::calculatePathDerivative(Path *path, Path *minPath, int nbLinks){
	std::list<StarLink*> list;
	Utils::getDisjointLinks(path, minPath, list, nbLinks);
	return calculatePathDerivativeForDisjoint(list);
	
};

FPType Utils::calculatePathDerivativeForDisjoint(const std::list<StarLink*> &list){
	StarLink* link = NULL;
	FPType der = 0.0;
	for (std::list<StarLink*>::const_iterator it = list.begin(); it != list.end(); ++it) {
		link = *it;
		der += link->getDerivative();
	}
	return der;
};

FPType Utils::calculatePathDerivativeForDisjoint(const std::list<StarLink*> &list1,
												 const std::list<StarLink*> &list2){
	return calculatePathDerivativeForDisjoint(list1) + calculatePathDerivativeForDisjoint(list2);
};

void Utils::readART3Coordinates(const std::string& art3NodesFile, 
	std::list<Coords>& newCoords){
	FileReader read(art3NodesFile);
	std::string line("");
	Coords node;
	node.nodeID = -1;
	node.x = -1;
	node.y = -1;
	int nbParams = 8;
	std::string params[nbParams]; 
	for (int i = 0; i < nbParams; ++i) 
		params[i] = "";
	while (read.isGood()) {
		line = read.getNextLine();
		if (!Utils::deleteWhiteSpaces(line).empty()){
			tokenizeIntoStr(line, params, nbParams);
			node.nodeID = atoi(params[1].c_str());
			node.x = atoi(params[2].c_str());
			node.y = atoi(params[3].c_str());
			newCoords.push_back(node);
		}
	}
};

void Utils::tokenizeIntoStr(const std::string& lineToTokenize, std::string* result, int nbTokens){
	std::stringstream ss(lineToTokenize);
	for (int i = 0; i < nbTokens; ++i) {
		ss >> result[i];
	}
};

void Utils::mapCoordsFromART3(const std::list<Coords>& coords, StarNetwork* net, 
	std::list<Coords>& newCoords){
	Coords node;
	node.nodeID = -1;
	node.x = -1;
	node.y = -1;
	for (int i = 0; i < net->getNbNodes(); ++i){
		int id = net->findID(i);
		if (id != -1) {
			node = Utils::findCoords(coords, id);
			if (node.nodeID == -1) {
				std::stringstream ss;
				ss << "Cannot find id = " << id;
				std::cout << ss.str() << std::endl;
			} else {
				newCoords.push_back(node);
			}
		} 
	}

};

void Utils::writeCoordsToFile(const std::list<Coords>& coords, const std::string& fileName){
	FileWriter write(fileName);
	for (std::list<Coords>::const_iterator it = coords.begin(); it != coords.end(); ++it) {
		std::stringstream ss("");
		ss << it->nodeID << " " << it->x << " " << it->y << "\n";
		write.writeLine(ss.str());
	}
};

Coords Utils::findCoords(const std::list<Coords>& coords, int nodeID) {
	for (std::list<Coords>::const_iterator it = coords.begin(); it != coords.end(); ++it){
		if (it->nodeID == nodeID) {
			return *it;
		}
	}
	Coords emptyNode;
	emptyNode.nodeID = -1;
	emptyNode.x = -1;
	emptyNode.y = -1;
	return emptyNode;
};

FPType Utils::generateRndNumber(FPType maxVal){
	return static_cast <FPType> (rand()) / (static_cast <FPType> (RAND_MAX/maxVal));
}
