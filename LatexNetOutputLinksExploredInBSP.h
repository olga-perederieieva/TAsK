#ifndef LATEX_NET_OUTPUT_LINKS_SEEN_IN_BSP
#define LATEX_NET_OUTPUT_LINKS_SEEN_IN_BSP 

#include "LatexNetOutput.h"

class BiObjLabelContainer;

/** \brief Implements creation of latex files to display links explored by
	bi-objective shortest path algorithm.

	All explored links are plotted in red, all other links are shown in blue.
	Next to each node number of non-dominated labels is shown.
*/
class LatexNetOutputLinksExploredInBSP : public LatexNetOutput {
	public:
		LatexNetOutputLinksExploredInBSP(StarNetwork* net, 
							const std::vector<StarLink*>& seenLinks,
							const BiObjLabelContainer& labels);
		LatexNetOutputLinksExploredInBSP(StarNetwork* net, 
							const std::vector<StarLink*>& seenLinks,
							const BiObjLabelContainer& labels,
			 				const std::string& logFileForMissingIDs);
		~LatexNetOutputLinksExploredInBSP();

	protected:
		virtual std::string getLineLabel(StarLink* link, const std::string& aboveOrBelow);
		virtual std::string generateLineOptions(const FlowInfo& flowInfo, StarLink* link);
		virtual bool plotLink(StarLink* link) const;
		virtual std::string getNodeLabel(int nodeID) const;

	private:
		const std::vector<StarLink*>& seenLinks_;
		const BiObjLabelContainer& labels_;

};
#endif