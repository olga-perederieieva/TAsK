#ifndef KML_SELECT_LINK
#define KML_SELECT_LINK 

#include "KMLNetOutput.h"

class PathSet;
class Path;

/** \brief Implements creation of kml-files for select link analysis.

	Highlights paths that go through a given link.
*/
class KMLSelectLink : public KMLNetOutput {
	public:
		KMLSelectLink(StarNetwork* net, const PathSet& paths);
		KMLSelectLink(StarNetwork* net, const PathSet& paths, 
			 			const std::string& logFileForMissingIDs);
		~KMLSelectLink();

		/** Sets link index for which all paths are shown.
		*/
		void setLinkIndex(int linkIndex);

	protected:
		/** @return link index.
		*/
		virtual FPType calcLineWidth(StarLink* link);
		/** @return colour of the link depending on its index.
		*/
		virtual std::string createColorFromLineWidth(FPType lineWidth);
		/** @return "5" if link has red colour, "3" otherwise.
		*/
		virtual std::string createLineWidth(FPType lineWidth);
		/** @return true.
		*/
		virtual bool shouldCreatePlacemark(StarLink* link);
		virtual std::string createDescriptionString(StarLink* link);

	private:
		int currentLiknIndex_;
		const PathSet& paths_;
		std::vector<std::string> coloredLinks_;
		std::vector<int> linkDescriptions_;

		void colorLinks();
		bool pathHasSelectedLink(Path* path) const;
		void colorLinksOfPath(Path* path, const std::string& decription);
		void initialiseColoredLinks();
};

#endif