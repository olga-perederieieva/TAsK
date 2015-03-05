#ifndef INDEFF_CURVE_CONTAINER
#define INDEFF_CURVE_CONTAINER

#include "IndiffCurve.h"

#include <vector>
#include <set>

class ODMatrix;
class OneSourceBiObjShPath;

/** \brief This class is a container of scalarization functions.
	It is responsible for their deallocation.
*/
class IndiffCurveContainer {
	public:
		explicit IndiffCurveContainer(int nbODpairs);
		virtual ~IndiffCurveContainer();

		/** This method adds curve \b curve to the internal container in index \b odPairIndex.
			If this index
			is invalid (negative or higher than number of O-D pairs) or if there is 
			a scalarization function assigned to this index, the assertion will stop the program.
		*/
		void addCurve(int odPairIndex, IndiffCurve* curve);

		/** @return pointer to a scalarization function of O-D pair \b odPairIndex.
		*/
		IndiffCurve* getCurve(int odPairIndex) const;

		/** Deallocates previous  scalarization functions (if there are any) and generate 
			random ones.
			@param maxNbPointsPerCurve maximum number of breakpoints per curve.
			@param maxToll maximum value of toll.
			@param maxTime maximum value of time.
		*/
		void generateRandomCurves(int maxNbPointsPerCurve, TollType maxToll, FPType maxTime);

		/** Deallocates previous scalarization functions (if there are any) and generate 
			random ones based on efficient paths.
			@param maxNbPointsPerCurve maximum number of breakpoints per curve.
			@param shPath one-source bi-objective shortest path algorithm.
			@param mat O-D matrix.
		*/
		void generateRandomCurvesWithBSP(int maxNbPointsPerCurve, OneSourceBiObjShPath& shPath,
				const ODMatrix& mat);

		/** Prints all scalarization functions on screen.
		*/
		void print() const;
		/** Writes scalarization functions in to file specified in fileName.
		*/
		void writeToFile(const std::string& fileName, const ODMatrix& mat) const;

	private:

		std::vector<IndiffCurve*> curves_;

		/** Deallocates all scalarization functions.
			\warning Doesn't work correctly if the same pointer to was added twice.
		*/
		void deallocateCurves();

		/** Creates scalarization function.
			@param maxNbPointsPerCurve maximum number of breakpoints per curve.
			@param maxToll maximum value of toll.
			@param maxTime maximum value of time.
			@param minTime minimum value of time.
			@return allocated scalarization function.
		*/
		virtual IndiffCurve* createCurve(int maxNbPointsPerCurve, TollType maxToll, 
					FPType maxTime, FPType minTime);

		/** Generates random tolls and writes them into randomTolls. All tolls are in the
			interval [1, maxToll]. All tolls are unique.
			@param nbPoints number of tolls that must be generated.
		*/
		void generateRandomTolls(std::set<TollType>& randomTolls, TollType maxToll, int nbPoints);
};

#endif