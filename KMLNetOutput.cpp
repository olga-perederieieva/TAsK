#include "KMLNetOutput.h"
#include "StarNetwork.h"
#include "FileWriter.h"
#include "StarLink.h"

#include <iomanip>
#include <sstream>
#include <iostream>

KMLNetOutput::KMLNetOutput(StarNetwork* net) : NetOutput(net) {

};

KMLNetOutput::KMLNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs) : 
			NetOutput(net, logFileForMissingIDs) {

};

KMLNetOutput::~KMLNetOutput(){

};

void KMLNetOutput::createKML(const std::string& fileWithNodes, const std::string& kmlFileName){

	std::cout << "KML will be written to: " << kmlFileName << std::endl;
	int nbNodes = net_->getNbNodes();
	std::vector<FPType> xCoord(nbNodes, 0);
	std::vector<FPType> yCoord(nbNodes, 0);
	std::vector<int> nodeID(nbNodes, 0);
	
	readCoord(fileWithNodes, xCoord, yCoord, nodeID);

	FileWriter writeKml(kmlFileName);
	writeKml.writeLine(createKmlHeader());
	
	for (StarLink* link = net_->beginOnlyLink(); link != NULL; link = net_->getNextOnlyLink()) {
		if (shouldCreatePlacemark(link)) {
			int tail = link->getNodeFromIndex();
			int head = link->getNodeToIndex();
			FPType x1 = xCoord[tail];
			FPType y1 = yCoord[tail];
			FPType x2 = xCoord[head];
			FPType y2 = yCoord[head];
			if (x1 == 0 && y1 == 0) std::cout << "Missing node coordinate: " << link->getNodeFrom() << 
								" link: " << link->toString() << std::endl; 
			if (x2 == 0 && y2 == 0) std::cout << "Missing node coordinate: " << link->getNodeTo() << 
								" link: " << link->toString() << std::endl;
			if (x1 != 0 && y1 != 0 && x2 != 0 && y2 != 0) 
				writeKml.writeLine(createPlacemark(x1, y1, x2, y2, link)); 
		}
	}

	writeKml.writeLine(createKmlFooter());
};

bool KMLNetOutput::shouldCreatePlacemark(StarLink* link) {
	return link->getFlow() > 0.0;
};

std::string KMLNetOutput::createKmlHeader() {
	std::string header;
	header.append("<?xml version=\"1.0\" encoding=\"utf-8\"?> \n");
	header.append("<kml xmlns=\"http://earth.google.com/kml/2.2\"> \n");
	header.append("\t<Document>\n");
    header.append("\t\t<name>Transportation network: " + net_->getNetName() + "</name>\n"); 
 	header.append("\t\t<Style id=\"styleEmpty\">\n");
    header.append("\t\t\t<IconStyle>\n"); 
    header.append("\t\t\t\t<Icon></Icon>\n"); 
    header.append("\t\t\t</IconStyle>\n"); 
    header.append("\t\t\t<LabelStyle>\n"); 
    header.append("\t\t\t\t<scale>1.1</scale> \n");
    header.append("\t\t\t</LabelStyle> \n");
    header.append("</Style>\n");
    return header;
};

std::string KMLNetOutput::createKmlFooter(){
	return ("\t</Document>\n</kml>\n");
};

std::string KMLNetOutput::createPlacemark(FPType x1, FPType y1, FPType x2,
		FPType y2, StarLink* link){ 
	std::stringstream ss;

	int linkIndex = link->getIndex();
	std::string placemark;
	placemark.append(createStyleForLink(linkIndex, calcLineWidth(link)));

	placemark.append("<Placemark>\n");
	
	placemark.append(createDescription(link));

	ss << "\t<styleUrl>#style" << linkIndex << "</styleUrl>\n";
	placemark.append(ss.str());
    placemark.append("\t\t<LineString>\n");
    placemark.append("\t\t\t" + createCoordLine(x1, y1, x2, y2)); 
    placemark.append("\t\t</LineString> \n");
    placemark.append("</Placemark> \n");

    placemark.append("<Placemark>\n");
    ss.str("");
    placemark.append("\t<styleUrl>#styleEmpty</styleUrl>\n");
    placemark.append("<Point>\n");
    ss.str("");
    ss << "<coordinates> " << x1 << ", " << y1 << ", 0 </coordinates> \n";
    placemark.append(ss.str());
    placemark.append("</Point>\n");
	placemark.append("</Placemark> \n");    
    return placemark;
};

std::string KMLNetOutput::createStyleForLink(int linkIndex, FPType lineWidth){
	std::stringstream ss;
	ss << "\t\t<Style id=\"style" << linkIndex << "\">\n";
	std::string styleString(ss.str());
    styleString.append("\t\t\t<IconStyle>\n"); 
    styleString.append("\t\t\t\t<Icon></Icon>\n"); 
    styleString.append("\t\t\t</IconStyle>\n"); 
    styleString.append("\t\t\t<LabelStyle>\n"); 
    styleString.append("\t\t\t\t<scale>1.1</scale> \n");
    styleString.append("\t\t\t</LabelStyle> \n");
   	styleString.append("\t\t\t<LineStyle> \n");

    styleString.append("\t\t\t\t<color>" + createColorFromLineWidth(lineWidth) + "</color> \n");
    styleString.append("\t\t\t\t<width>" + createLineWidth(lineWidth) + "</width> \n");
   	
   	styleString.append("\t\t\t</LineStyle> \n");
    styleString.append("\t\t</Style> \n");

    return styleString;
};

std::string KMLNetOutput::createColorFromLineWidth(FPType lineWidth){
	if (lineWidth > 1.0) {
    	return getRedColor();
    } else if (lineWidth == -1.0) {
    	return getGreenColor();
    }
   	return getBlueColor();
};

std::string KMLNetOutput::createLineWidth(FPType lineWidth){
	std::stringstream ss;
	if (lineWidth < 0.0) lineWidth  = 1.0;
    ss << 3*lineWidth;
    return ss.str();
}

FPType KMLNetOutput::calcLineWidth(StarLink* link){
	FPType capacity = (link->getLinkFnc())->getCapacity();
	FPType lineWidth = link->getFlow() / capacity;
	if (capacity == 0.0) {
		return -1;
	}
	return lineWidth;
};

std::string KMLNetOutput::createDescription(StarLink* link) {
	std::string description("");
	description.append("\t<name>" + createLinkNameForDescription(link) + "</name>\n");
	description.append("\t<description>" + createDescriptionString(link) + "</description>\n");
	return description;
};

std::string KMLNetOutput::createLinkNameForDescription(StarLink* link) {
	std::stringstream ss;
	ss << link->getIndex();
	return ss.str();
};

std::string KMLNetOutput::createDescriptionString(StarLink* link){
	LinkFnc* fnc = link->getLinkFnc();
	FPType ratio = 0;
	if (fnc->getCapacity() != 0)  ratio = link->getFlow() / fnc->getCapacity();
	std::stringstream ss;
	ss << fnc->toString() << ", flow: " << link->getFlow() << ", time: " << 
		link->getTime() << ", capacity: " << fnc->getCapacity() << 
		", flow/capacity = " << ratio
		 << " from: " << link->getNodeFrom() << ", to: " << link->getNodeTo();
	return ss.str();
};

std::string KMLNetOutput::createCoordLine(FPType x1, FPType y1, FPType x2, FPType y2) {
	std::stringstream ss;
	int precision = 10;
	ss << "<coordinates>" << std::setprecision(precision) << x1 << "," << 
    	std::setprecision(precision) << y1 << ",0"; 
    if (x2 != std::numeric_limits<FPType>::infinity()) 
    	ss << "\n" << std::setprecision(precision) << x2 << "," << 
    	std::setprecision(precision) << y2 << ",0";
    ss << "</coordinates> \n";
    return ss.str();
};