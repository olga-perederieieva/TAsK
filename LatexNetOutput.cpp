#include "LatexNetOutput.h"
#include "FileWriter.h"
#include "StarNetwork.h"
#include "StarNode.h"
#include "StarLink.h"

#include <sstream>
#include <math.h>
#include <iostream>

#define COORD_TOL 1e-2

LatexNetOutput::LatexNetOutput(StarNetwork* net) : NetOutput(net) {
	setPlotInfoToDefault();
};

LatexNetOutput::LatexNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs) : 
			NetOutput(net, logFileForMissingIDs) {
	setPlotInfoToDefault();
};

LatexNetOutput::~LatexNetOutput(){

};

void LatexNetOutput::setPlotInfoToDefault(){
	snInfo_.side1 = "south";
	snInfo_.side2 = "north";
	snInfo_.shift = "xshift";

	weInfo_.side1 = "west";
	weInfo_.side2 = "east";
	weInfo_.shift = "yshift";
};

void LatexNetOutput::printToLaTeX(const std::string& fileWithNodes, const std::string& texFile, 
		bool addHeader, FPType scale) { 
	FileWriter outputFile(texFile);
	std::cout << "file name: " << texFile << std::endl;
	
	if (addHeader) {
		outputFile.writeLine("\\documentclass[a4paper]{article} \n");
		outputFile.writeLine("\\usepackage{a4wide} \n");
		outputFile.writeLine("\\usepackage{tikz} \n \\usetikzlibrary{calc} \n");
		outputFile.writeLine("\\begin{document} \n");
	}
	std::stringstream ss;
	ss << " \\begin{tikzpicture}[scale=" << scale << "] \n";
	outputFile.writeLine(ss.str());
	
	int nbNodes = net_->getNbNodes();
	std::vector<FPType> xCoord(nbNodes, 0);
	std::vector<FPType> yCoord(nbNodes, 0);
	
	std::vector<int> nodeID(nbNodes, 0);
	
	readCoord(fileWithNodes, xCoord, yCoord, nodeID);

	addTikzNodes(outputFile, xCoord, yCoord, nodeID);

	FlowInfo flowInfo = getFlowInfo();
	
	for (StarNode* node = net_->beginNode(); node != NULL; node = net_->getNextNode()){
		for (StarLink* link = net_->beginLink(); link != NULL; link = net_->getNextLink()) {
			
			if (plotLink(link)) {
				int tail = node->getIndex();
				int head = link->getNodeToIndex();
				std::string lineOptions = generateLineOptions(flowInfo, link);
				
				std::string oneLine("");
				setPlotInfoToDefault();
				if (onHorizontalLine(tail, head, yCoord)) {
					oneLine = drawHorizontalLink(link, lineOptions, (xCoord[tail] < xCoord[head]));
				} else if (onVerticalLine(tail, head, xCoord)) {
					oneLine = drawVerticalLink(link, lineOptions, (yCoord[tail] < yCoord[head]));
				} else {
					oneLine = drawDiagonalLink(link, lineOptions, (xCoord[tail] < xCoord[head]),
						(yCoord[tail] < yCoord[head]));
				}
				if (!oneLine.empty()) outputFile.writeLine(oneLine);

			}
		
		} 
	}	
	
	
	outputFile.writeLine(" \\end{tikzpicture} \n \n");
	
	if (addHeader) outputFile.writeLine("\\end{document} \n");
	
};

bool LatexNetOutput::plotLink(StarLink* link) const {
	return link->getFlow() > 0.0;
};

void LatexNetOutput::addTikzNodes(FileWriter& outputFile, const std::vector<FPType>& xCoord, 
				const std::vector<FPType>& yCoord, const std::vector<int>& nodeID){
	int nbNodes = net_->getNbNodes();
	for (int i = 0; i < nbNodes; ++i) {
		if (xCoord[i] != -1 && yCoord[i] != -1) {
			std::stringstream tmpss;
			tmpss << "\\node[draw,thick,circle,black,minimum size=0.75cm] (n" << nodeID[i] << ") at (" << xCoord[i] << "," 
				<< yCoord[i] << ") {" << getNodeLabel(nodeID[i]) << "}; \n"; //
			outputFile.writeLine(tmpss.str());
		}
	}
};

std::string LatexNetOutput::getNodeLabel(int nodeID) const {
	std::stringstream ss;
	ss << nodeID;
	return ss.str();
};

bool LatexNetOutput::onHorizontalLine(int tail, int head, const std::vector<FPType>& yCoord){
	return (fabs(yCoord[tail] - yCoord[head]) <= COORD_TOL); 
};

bool LatexNetOutput::onVerticalLine(int tail, int head, const std::vector<FPType>& xCoord){
	return (fabs(xCoord[tail] - xCoord[head]) <= COORD_TOL); 
};

std::string LatexNetOutput::getLineLabel(StarLink* link, const std::string& aboveOrBelow){
	std::stringstream ss;
	ss << "-- node[sloped," + aboveOrBelow + "] {" <<  
			link->getTime() << "}";
	return ss.str();
};

std::string LatexNetOutput::drawHorizontalLink(StarLink* link, const std::string& lineOptions, 
		bool tailIsLeft){
	
	int tailShiftVal = getShift(weInfo_, tailIsLeft);
	std::string tailShift = createShiftStr(weInfo_.shift, tailShiftVal);	
	std::stringstream ss;
	std::string aboveOrBelow("below");
	if (tailIsLeft) aboveOrBelow = "above";
	ss << "\\draw[" << lineOptions <<"] ([" << tailShift << "]n" << link->getNodeFrom() 
					<<	"." << weInfo_.side1 << ") " << getLineLabel(link, aboveOrBelow) << 
					" ([" << tailShift << "]n" << 
					link->getNodeTo() << "." << weInfo_.side2 << "); \n";
	return ss.str();
};

 std::string LatexNetOutput::drawVerticalLink(StarLink* link, const std::string& lineOptions, 
 		bool tailIsUp){
 	int headShiftVal = getShift(snInfo_, tailIsUp);
 	std::string headShift = createShiftStr(snInfo_.shift, headShiftVal);
 	std::stringstream ss;
 	ss << "\\draw[" << lineOptions <<"] ([" << headShift << "]n" << link->getNodeFrom() 
					<<	"." << snInfo_.side1 << ") " << getLineLabel(link, "above") << " ([" << headShift << "]n" << 
					link->getNodeTo() << "." << snInfo_.side2 << "); \n";
	return ss.str();
 };

 std::string LatexNetOutput::drawDiagonalLink(StarLink* link, const std::string& lineOptions, 
 		bool tailIsLeft, bool tailIsUp){
 	int tailShiftVal = getShift(weInfo_, tailIsLeft);
	std::string tailShift = createShiftStr(weInfo_.shift, tailShiftVal);
	int headShiftVal = getShift(snInfo_, tailIsUp);
 	std::string headShift = createShiftStr(snInfo_.shift, headShiftVal);
 	std::stringstream ss;
 	std::string aboveOrBelow("below");
	if (tailIsLeft) aboveOrBelow = "above";
 	ss << "\\draw[" << lineOptions <<"] ([" << headShift << "," << tailShift << "]n" << 
 					link->getNodeFrom() 
					<<	"." << snInfo_.side1 << ") " << getLineLabel(link, aboveOrBelow) 
					<<  " ([" << headShift << "," << 
						tailShift << "]n" << 
					link->getNodeTo() << "." << snInfo_.side2 << "); \n";
 	return ss.str();
 };

 FlowInfo LatexNetOutput::getFlowInfo(){
	FPType totalFlow = 0.0;
	FPType minFlow = std::numeric_limits<FPType>::max( );
	FPType maxFlow = 0.0;
	for (StarLink* link = net_->beginOnlyLink(); link != NULL; link = net_->getNextOnlyLink()) {
		FPType flow = link->getFlow();
		if (flow < minFlow) {
			minFlow = flow;
		}
		if (flow > maxFlow) {
			maxFlow = flow;
		}
		totalFlow += flow;
	}
	FlowInfo flowInfo;
	flowInfo.minFlow = minFlow;
	flowInfo.maxFlow = maxFlow;
	flowInfo.totalFlow = totalFlow;
	return flowInfo;
};

std::string LatexNetOutput::generateLineOptions(const FlowInfo& flowInfo, StarLink* link){
	std::stringstream sstm;
	FPType tmpFlow = (link->getFlow() - flowInfo.minFlow) / (flowInfo.maxFlow - flowInfo.minFlow);
	FPType capacity = (link->getLinkFnc())->getCapacity();
	std::string color("blue");
	if (link->getFlow() / capacity > 1) {
		color = "red";
	}
	double lineWidth = 0.5 + 5 * tmpFlow - 0.5 * tmpFlow;
	sstm << "line width=" << lineWidth << ", " << color << ", ->";
	return sstm.str();
};

int LatexNetOutput::getShift(PlotInfo& node, bool swapValues){
	int shiftVal = 5;
	if (swapValues) {
		std::swap(node.side1, node.side2);
		return -shiftVal;
	}
	return shiftVal;
}

std::string LatexNetOutput::createShiftStr(const std::string& shift, int val){
	std::stringstream ss;
	ss << shift << "=" << val << "pt";
	return ss.str();
};