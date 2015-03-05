#ifndef KML_PLOT_TOLLS
#define KML_PLOT_TOLLS 

#include "KMLNetOutput.h"

/** \brief This class creates kml-file where tolled links are highlighted in red.
*/
class KMLPlotTolls : public KMLNetOutput {
	public:
		KMLPlotTolls(StarNetwork* net, const TollContainerType& tolls);
		KMLPlotTolls(StarNetwork* net, const std::string& logFileForMissingIDs,
			const TollContainerType& tolls);
		~KMLPlotTolls();

	protected:
		/** @return -1 if toll is zero, 0 otherwise.
		*/
		virtual FPType calcLineWidth(StarLink* link);
		/** @return blue if lineWidth < 0, red otherwise.
		*/
		virtual std::string createColorFromLineWidth(FPType lineWidth);
		/** @return "3".
		*/
		virtual std::string createLineWidth(FPType lineWidth);
		/** @return true.
		*/
		virtual bool shouldCreatePlacemark(StarLink* link);
		virtual std::string createDescriptionString(StarLink* link);

	private:
		const TollContainerType& tolls_;

};
#endif