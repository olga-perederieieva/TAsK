#ifndef LABEL_CORRECTING_POINT_2_POINT_BSP
#define LABEL_CORRECTING_POINT_2_POINT_BSP

#include "LabelSettingBSPBase.h"
#include "Point2PointBiObjShPath.h"
#include "BoundsCalculatorForBSPBase.h"

class NonAdditivePC;
class PathsAdder;
class DominationByPathCostBase;

/** \brief Concrete implementation of point-to-point bi-objective label-setting algorithm.
*/
class LabelSettingPoint2PointBSP : public Point2PointBiObjShPath, 
					protected LabelSettingBSPBase	{
	public:

		/** @param net network.
			@param tolls container with link tolls.
			@param bounds objects that knows how to calculate toll and time 
				lower bounds.
			@param pathAdder objects that adds paths to the destination node.
			@param dominanceRule object that knows how to apply path cost domination rule.
		*/
		LabelSettingPoint2PointBSP(StarNetwork& net, const TollContainerType& tolls, 
						BoundsCalculatorForBSPBase& bounds,
						const PathsAdder& pathAdder, 
						DominationByPathCostBase& dominanceRule);
		~LabelSettingPoint2PointBSP();

		virtual void calculate(int originIndex, int destIndex, int odIndex);
		virtual void createPath(BiObjLabel *labelWithMinCost, std::list<StarLink*>& path);

	protected:

		int currOdIndex_;
		DominationByPathCostBase& dominanceRule_;

		/** Implements special stopping condition for point-to-point shortest path, 
			see \cite DemeyerGAPD13.
		*/
		virtual bool stillLookingForLabels(const QueueType& tmpLabels) const;
		/** Calculates estimated time and toll for the label with cost vector [newTime, newToll]
			and calls domination rule, see isWorth().
		*/
		virtual bool isWorthAdding(FPType newTime, TollType newToll, int nodeIndex) const;
		/** @return destination index for which algorithm was called most recently.
		*/
		int getDestIndex() const;

		/** @return true if it is worth to keep label with estimated cost vector
				[timeLowerBound, tollLowerBound], false otherwise.
		*/
		bool isWorth(FPType timeLowerBound, TollType tollLowerBound) const;
		/** Calls uploadPaths on pathAdder_ object and updateBestKnown on dominanceRule_.
		*/
		virtual void uploadPaths();
		/** Calls addLabelToLabelSet method of dominanceRule_.
		*/
		virtual void addLabelToLabelSet(BiObjLabelContainer& labels, int nodeIndex, 
				BiObjLabel* label);


	private:
		int destNode_;
		BoundsCalculatorForBSPBase& bounds_;
		const PathsAdder& pathAdder_;

};

#endif
