#ifndef DIFF_KML_NET_OUTPUT
#define DIFF_KML_NET_OUTPUT

#include "KMLNetOutput.h"

/** \brief This class implements KML files that describe link flow differences between solutions
	(usually two solutions are compared to each other).
*/
class DiffKMLNetOutput : public KMLNetOutput {
	public:
		/** @param net network.
			@param flowDiff vector with link flow differences.
			@param tolerance describes what link flow differences should be considered zero.
		*/
		DiffKMLNetOutput(StarNetwork* net, const std::vector<FPType>& flowDiff, FPType tolerance);
		DiffKMLNetOutput(StarNetwork* net, const std::string& logFileForMissingIDs, 
			const std::vector<FPType>& flowDiff, FPType tolerance);
		~DiffKMLNetOutput();

	protected:
		virtual FPType calcLineWidth(StarLink* link);
		virtual std::string createColorFromLineWidth(FPType lineWidth);
		virtual std::string createLineWidth(FPType lineWidth);
		virtual bool shouldCreatePlacemark(StarLink* link);
		virtual std::string createDescriptionString(StarLink* link);

	private:
		const std::vector<FPType>& flowDiff_;
		FPType scaleFactor_;
		const FPType tolerance_;
		static const int maxLength_ = 10;

		/** Calculates scaling factor for link width. This value depends on maximum 
			absolute link flow difference.
		*/
		void calcScalingFactor();

};
#endif