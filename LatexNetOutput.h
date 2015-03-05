#ifndef LATEX_NET_OUTPUT
#define LATEX_NET_OUTPUT

#include "NetOutput.h"

class StarLink;
class FileWriter;

struct FlowInfo {
	FPType minFlow;
	FPType maxFlow;
	FPType totalFlow;
};

struct PlotInfo {
	std::string side1;
	std::string side2;
	std::string shift;	
};

/** \brief This class is responsible for creation of latex files to plot
	link flows. Node coordinates must be known in order to create a latex
	file.

	The algorithm plots network performance graphs, i.e. congested links are red,
	not congested links are blue, width of each link is proportional to the
	ratio flow/capacity. Only links with positive flow are plotted.
*/
class LatexNetOutput : public NetOutput {
	public:
		LatexNetOutput(StarNetwork* net);
		LatexNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs);
		~LatexNetOutput();

		/** @param fileWithNodes path to file with node coordinates.
			@param texFile path to file where latex code will be written.
			@param addHeader if set to true latex header is added to file, if set to 
				false only code for tikzpicture is written to file.
			@param scale \begin{tikzpicture}[scale = scale].
		*/
		void printToLaTeX(const std::string& fileWithNodes, const std::string& texFile, 
			bool addHeader, FPType scale);

	protected:
		/** @return line label that describes link travel time.
		*/
		virtual std::string getLineLabel(StarLink* link, const std::string& aboveOrBelow);
		/** @return options for a given link, e.g. line width and colour.
		*/
		virtual std::string generateLineOptions(const FlowInfo& flowInfo, StarLink* link);
		/** @return true if link flow is positive, false otherwise.
		*/
		virtual bool plotLink(StarLink* link) const;
		/** @return node ID.
		*/
		virtual std::string getNodeLabel(int nodeID) const;

	private:
		PlotInfo snInfo_;
		PlotInfo weInfo_;

		void addTikzNodes(FileWriter& outputFile, const std::vector<FPType>& xCoord, const std::vector<FPType>& yCoord, 
				const std::vector<int>& nodeID);

		void setPlotInfoToDefault();
 		FlowInfo getFlowInfo();
		
		bool onHorizontalLine(int tail, int head, const std::vector<FPType>& yCoord);
		bool onVerticalLine(int tail, int head, const std::vector<FPType>& xCoord);
		std::string drawHorizontalLink(StarLink* link, const std::string& lineOptions, 
			bool tailIsLeft);
		std::string drawVerticalLink(StarLink* link, const std::string& lineOptions, 
 			bool tailIsUp);
		std::string drawDiagonalLink(StarLink* link, const std::string& lineOptions, 
 			bool tailIsLeft, bool tailIsUp);

		std::string createShiftStr(const std::string& shift, int val);
		int getShift(PlotInfo& node, bool swapValues);
		
};

#endif