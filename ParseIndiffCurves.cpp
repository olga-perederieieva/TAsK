#include "ParseIndiffCurves.h"
#include "FileReader.h"
#include "PieceWiseLinearIndiffCurve.h"
#include "UtilsString.h"
#include "Error.h"
#include "IndiffCurveContainer.h"

#include <sstream>
#include <cassert>

ParseIndiffCurves::ParseIndiffCurves() {

};

ParseIndiffCurves::~ParseIndiffCurves() {

};

int ParseIndiffCurves::getODIndex(const std::string& line) {
	size_t pos = line.find("(");
	assert(pos != std::string::npos);
	std::string first3numbers = line.substr(0, pos - 1); // -1 to delete the whitespace
	int originIndex = -1;
	int destIndex = -1;
	int odIndex = -1;
	std::stringstream ss(first3numbers);
	ss >> originIndex >> destIndex >> odIndex;
	assert(odIndex != -1);
	return odIndex;
};

FPType ParseIndiffCurves::getFirst(const std::string& line) {
	size_t pos = line.find(",");
	assert(pos != std::string::npos);
	return strtold(line.substr(0, pos).c_str(), NULL);
};

FPType ParseIndiffCurves::getSecond(const std::string& line)  {
	size_t pos = line.find(",");
	assert(pos != std::string::npos);
	return strtold(line.substr(pos + 1).c_str(), NULL);
};

int ParseIndiffCurves::getCurvePoints(const std::string& line, PointsContainer& points) {
	int count = 0;
	std::string tmpLine(line);
	while (true) {
		try {
			std::string substr = Utils::getSubString("(", ")", tmpLine);
			assert(!substr.empty());
			points.push_back(std::make_pair<FPType, FPType>(getFirst(substr), getSecond(substr)));
			++count;
			tmpLine = tmpLine.substr(tmpLine.find(")") + 1);
		} catch (Error e) {
			return count;
		}
	}
	return count;
};

IndiffCurve* ParseIndiffCurves::createCurve(int nbPoints, const PointsContainer& points,
	const std::string& curveType) {
	assert(curveType == "");
	PieceWiseLinearIndiffCurve* curve = new PieceWiseLinearIndiffCurve(nbPoints);
	for (PointsContainer::const_iterator it = points.begin(); it != points.end(); ++it) {
		curve->addPoint(it->first, it->second);
	}
	return curve;
};

std::string ParseIndiffCurves::getCurveType(const std::string& line) {
	if (line.find("Linear") != std::string::npos) {
		return "Linear";
	} else if (line.find("Convex") != std::string::npos) {
		return "Convex";
	} else if (line.find("Concave") != std::string::npos) {
		return "Concave";
	}
	return "";
};

void ParseIndiffCurves::parse(const std::string& fileName, IndiffCurveContainer& container) {
	FileReader reader(fileName);

	while (reader.isGood()) {
		std::string line(reader.getNextLine());
		if (!Utils::skipOneLineComment("//", line).empty()) {
			int odIndex = getODIndex(line);
			PointsContainer points;
			int nbPoints = getCurvePoints(line, points);
			container.addCurve(odIndex, createCurve(nbPoints, points, getCurveType(line)));
		}
	}
};