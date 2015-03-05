#ifndef NET_OUTPUT
#define NET_OUTPUT

class StarNetwork;

#include "UsedTypes.h"

#include <vector>
#include <string>

/** \brief This calss is a base class for the classes that perform some output work on StarNetwork 
	object (create KML, create LaTeX, etc.)
	\details It contains basic functionality used by derived classes.
*/
class NetOutput {
	public:
		virtual ~NetOutput();

		NetOutput(StarNetwork* net, const std::string& logFileForMissingIDs = "log.missing.ids.txt");
		
		/** Reads coordinates from file \b file and writes the result to xCoord,
			yCoord and nodeID.
		*/
		void readCoord(const std::string& file, std::vector<FPType>& xCoord, 
				std::vector<FPType>& yCoord, std::vector<int>& nodeID);

	protected:
		StarNetwork* net_;
		std::string logFileForMissingIDs_;

		// IMPORTANT: these are KML colors, not hex
		static const std::string getRedColor() {
			return "7f0000ff";
		};

		static const std::string getBlueColor() {
			return "50f04614";
		};

		static const std::string getGreenColor() {
			return "5014b41e";
		};

		static const std::string getLightBlueColor(){
			return "50F0C814";
		};

		static const std::string getOrangeColor(){
			return "501478FF";
		};		

};

#endif