#ifndef PIECEWISE_LINEAR_INDIFF_CURVE
#define PIECEWISE_LINEAR_INDIFF_CURVE

#include "IndiffCurve.h"

/** \brief This class implements piecewise-linear scalarization function. */
class PieceWiseLinearIndiffCurve : public IndiffCurve {

	public:
		PieceWiseLinearIndiffCurve(int nbPoints);
		~PieceWiseLinearIndiffCurve();

		/** This method is used for scalarization function creation. 
			Adds point (\b toll, \b value) to the scalarization function. 
			\note Points must be added in the increasing order of toll. Points with 
			the same value of toll cannot be added. All values must be non-negative.
			The first value of toll must be zero.
			If these conditions are violated, the error will be thrown.
		*/
		void addPoint(TollType toll, FPType value);

		/** Evaluates the scalarization function given the value of toll.
			If toll is between two known points, the returned value corresponds to a line between 
			these two points. If toll is more than last known point, maximum value of FPType is returned.
			If toll is negative, assert will terminate the program.
		*/
		virtual FPType evaluate(TollType toll);
		/** @return string representation of the scalarization function.
		*/
		virtual std::string toString();
		/** @return total number of breakpoints of the scalarization function.
		*/
		int getNbPoints() const;

		/** Perturbs travel time values of the scalarization function.
			@param percentage percentage of perturbation, must be within interval [0, 1].
			@param probOfPerturbation probability of perturbation, must be within interval [0, 1].
		*/
		virtual void perturbTime(FPType percentage, FPType probOfPerturbation);
		/** Perturbs travel toll values of the scalarization function.
			@param percentage percentage of perturbation, must be within interval [0, 1].
			@param probOfPerturbation probability of perturbation, must be within interval [0, 1].
		*/
		virtual void perturbToll(FPType percentage, FPType probOfPerturbation);
		/** Restores not perturbed scalarization function.
		*/
		virtual void restoreOriginalCurve();

	private:
		const int nbPoints_;
		int size_;
		std::vector<FPType> values_;
		std::vector<TollType> tolls_;
		std::vector<FPType> valuesCopy_;
		std::vector<TollType> tollsCopy_;
	
};

#endif