#ifndef KML_NET_OUTPUT
#define KML_NET_OUTPUT

#include "NetOutput.h"

#include <limits>

class StarLink;

/** \brief KML for network performance analysis (colour-coded based on the ratio flow/capacity)
	
	This class creates kml-file with current flow solution. Different colours
	correspond to different levels of congestion (based on the ratio flow/capacity)
*/
class KMLNetOutput : public NetOutput {
	public:
		KMLNetOutput(StarNetwork* net);
		KMLNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs);
		virtual ~KMLNetOutput();

		/** Creates kml-file.
			@param fileWithNodes path to file with coordinates for each node (all
				coordinates must be in wgs84 coordinate system).
			@param kmlFileName path to file where kml should be written.
		*/
		void createKML(const std::string& fileWithNodes, const std::string& kmlFileName);

	protected:
		/** @return flow/capacity ratio, -1 if capacity is zero.
		*/
		virtual FPType calcLineWidth(StarLink* link);
		/** @return kml colour depending on line width: if lineWidth is -1, green colour,
				if lineWidth is greater than 1, red colour, blue colour otherwise.
		*/
		virtual std::string createColorFromLineWidth(FPType lineWidth);
		/** @return scaled line width for kml-output.
		*/
		virtual std::string createLineWidth(FPType lineWidth);
		/** @return true if link flow is positive, false otherwise.
		*/
		virtual bool shouldCreatePlacemark(StarLink* link);
		/** @return string for link description.
		*/
		virtual std::string createDescriptionString(StarLink* link);
		/** @return link name used in description.
		*/
		virtual std::string createLinkNameForDescription(StarLink* link);

	private:
		std::string createKmlHeader();
		std::string createKmlFooter();
		std::string createPlacemark(FPType x1, FPType y1, FPType x2,
			FPType y2, StarLink* link);
		std::string createStyleForLink(int linkIndex, FPType lineWidth);
		std::string createDescription(StarLink* link);
		std::string createCoordLine(FPType x1, FPType y1, 
			FPType x2 = std::numeric_limits<FPType>::infinity(), 
			FPType y2 = std::numeric_limits<FPType>::infinity());

};
#endif