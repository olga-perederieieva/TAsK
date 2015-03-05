#ifndef PARSE_INDIFF_CURVES
#define PARSE_INDIFF_CURVES 

#include "UsedTypes.h"

#include <string>
#include <list>

class IndiffCurveContainer;
class IndiffCurve;

/** \brief Parser for piecewise-linear scalarization functions and a base class
	for parsers of other types of functions.
*/
class ParseIndiffCurves {
	public:
		ParseIndiffCurves();
		virtual ~ParseIndiffCurves();
		/** @param fileName path to file with scalarization functions.
			@param container reference to container where scalarization functions will be added.
		*/
		void parse(const std::string& fileName, IndiffCurveContainer& container);

	protected:
		typedef std::list<std::pair<FPType, FPType> > PointsContainer;
		/** @return O-D pair index extracted from line.
		*/
		int getODIndex(const std::string& line);
		/** @return toll value extracted from line.
		*/
		FPType getFirst(const std::string& line);
		/** @return travel time value extracted from line.
		*/
		FPType getSecond(const std::string& line);
		/** Extracts points of the piecewise-linear scalarization function from line and
			stores them in points.
			@return total number of extracted points.
		*/
		int getCurvePoints(const std::string& line, PointsContainer& points);
		/** Allocates memory for a piecewise-linear scalarization function based on points
			and returns a pointer to it.
			@param nbPoints number of points.
			@param points all points.
			@points curveType type of curve.
		*/
		virtual IndiffCurve* createCurve(int nbPoints, const PointsContainer& points, 
			const std::string& curveType);
		/** @return string that describes type of curve, empty string if it is not 
				linear, convex or concave curve.
		*/
		std::string getCurveType(const std::string& line);
};

#endif