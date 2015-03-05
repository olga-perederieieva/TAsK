#ifndef OBJECT_MANAGER
#define OBJECT_MANAGER

#include "UsedTypes.h"
#include "Constants.h"

class DescDirectionPath;
class PathBasedFlowMove;
class StepSize;
class PathBasedFlowMoveWithStep;
class NonAdditivePC;
class Params;
class ShortestPath;
class StarNetwork;
class ODMatrix;
class DecoratedEqAlgo;
class LinkFncContainer;
class LinkFlows;
class LineSearch;
class ConvMeasure;
class Derivative;
class PathSet;
class PathCost;
class OriginSet;
class AddHook;
class AddHookStoreOut;
class PASManager;
class StepSizeAdd;
class PathBasedFlowMoveGP;
class LabelCorrectingAl;
class NonAddShortestPath;
class IndiffCurveContainer;
class OneSourceBiObjShPath;
class Point2PointBiObjShPath;
class BoundsCalculatorForBSPBase;
class PathsAdder;
class CurrentPathsAdder;
class Astar;
class BoundsCalculatorForBackwardNet;
class DominationByPathCostBase;
class BoundsCalculatorWithTimeUpdate;

/** \brief This class is responsible for creation of all objects. 
	\details It is suggested to use it when a certain object is needed. 
	It also ensures that only one instance of each object is created (by one ObjectManager instance).
*/
class ObjectManager {
	public:
		/** @param params object with all information about parameters necessary to create 
				proper objects.
		*/
		ObjectManager(Params *params);
		~ObjectManager();
		
		/** @return network.
		*/
		StarNetwork* getNet();
		/** @return O-D matrix.
		*/
		ODMatrix* getODMatrix();
		/** @param dirConv directory where convergence file should be stored. If not specified,
				the value from parameters will be used.
			@param dirFlows directory where file with link flows should be stored. If not specified,
				the value from parameters will be used.
			@return TA algorithm specified in parameters object.
		*/
		DecoratedEqAlgo* getEqAlgo(const std::string& dirConv = "",
			const std::string& dirFlows = "");
		/** @return container with link cost functions.
		*/
		LinkFncContainer* getLinkFncCont();
		/** @return line search algorithm.
		*/
		LineSearch* getLineSearch();
		/** @return convergence measure.
		*/
		ConvMeasure* getConvMeasure();
		/** @return single-objective shortest path algorithm.
		*/
		ShortestPath* getShPath();
		/** @return derivative object.
		*/
		Derivative* getDerivative();
		/** @return set of paths between each O-D pair.
		*/
		PathSet* getPathSet();
		/** @return object responsible for path cost calculation.
		*/
		PathCost* getPathCost();
		/** @return set of all bushes.
		*/
		OriginSet* getOriginSet();
		/** @return manager of PASs, see PASManager class.
		*/
		PASManager* getPASManager();
		
		/** @return container of all scalarization functions.
		*/
		IndiffCurveContainer* getIndiffCurveContainer();
		/** @return container of link tolls.
		*/
		TollContainerType* getTolls();

		/** @returns a FPType value of a parameter from configuration file if it was in the file.
			If specified parameter does not exist, the error is thrown.
		*/
		FPType getFloatValue(std::string paramName);

	private:
		Params *params_;
		
		int isAdditive_;
		PathAlgoType algoType_;
		PathApp algoApp_;
		
		StarNetwork *net_;
		ODMatrix *odMatrix_;
		DecoratedEqAlgo *algo_;
		LinkFncContainer *linkFnc_;
		LinkFlows *linkFlows_;
		LineSearch *lineSearch_;
		ConvMeasure *convMeasure_;
		ShortestPath *shPath_;
		Derivative *der_;
		PathSet *pathSet_;
		PathCost *pathCost_;
		OriginSet *originSet_;
		AddHook *addHook_;
		AddHookStoreOut *addHookStore_; /**< just a copy of addHook_, but explicit 
										type is specified */
		PASManager* pasManager_;
		DescDirectionPath* descDirPath_;
		PathBasedFlowMove* flowMove_;
		StepSize* stepSize_;
		StepSizeAdd* stepSizeAdd_; /**< just a copy of addHook_, but explicit type is specified */
		PathBasedFlowMoveWithStep* flowMoveWithStep_; /**< just a copy of addHook_, 
									but explicit type is specified */
		PathBasedFlowMoveGP* flowMoveGP_; /**< just a copy of addHook_, but explicit type
										 is specified */
		
		LabelCorrectingAl *LCShortestPath_;
		NonAddShortestPath* nonAddShPath_;
		NonAdditivePC* ndPathCost_;
		IndiffCurveContainer* indiffCurveCont_;
		TollContainerType* tolls_;
		OneSourceBiObjShPath* oneSourceBSP_;
		Point2PointBiObjShPath* point2pointBSP_;
		BoundsCalculatorForBSPBase* boundsCalculator_;
		PathsAdder* pathsAdder_;
		CurrentPathsAdder* currPathsAdder_;
		Astar* aStar_;
		BoundsCalculatorForBackwardNet* backwardBounds_;
		DominationByPathCostBase* dominationRule_;
		Astar* aStarForNonAdd_;
		BoundsCalculatorWithTimeUpdate* boundsWithTimeUpdate_;
		
		/** @return true if additive traffic assignment is used, false otherwise.
		*/
		bool getIfAdditive();
		/** @return type of path-based algorithm, for possible values see Constants.h.
		*/
		PathAlgoType getPathAlgoType();
		/** @return approach type of path-based algorithm, for possible values see Constants.h.
		*/
		PathApp getPathAlgoApp();
		/** @return pointer to object responsible for calculation of descent
				direction of path-based algorithm.
		*/
		DescDirectionPath* getDescDirectionPath();
		/** @return pointer to object responsible for extra operations with algorithm output
				(e.g. print on screen, write to file).
		*/
		AddHook* getAddHook();
		/** @return pointer to object responsible for shifting flow between paths in path-based 
				algorithm.
		*/
		PathBasedFlowMove* getPathBasedFlowMove();
		/** @return pointer to object responsible for shifting flow between paths in path-based 
				algorithm that uses line search for step size calculation.
		*/
		PathBasedFlowMoveWithStep* getPathBasedFlowMoveWithStep();
		/** @return pointer to object responsible for shifting flow between paths in 
				gradient projection algorithm.
		*/
		PathBasedFlowMoveGP* getPathBasedFlowMoveGP();
		/** @return pointer to object responsible for calculating an appropriate step size
				by means of a line search.
		*/
		StepSize* getStepSize();
		/** @return same as getStepSize(), but returned object works for additive 
				traffic assignment only.
		*/
		StepSizeAdd* getStepSizeAdd();
		/** @return pointer to object that implements non-additive shortest path algorithm.
		*/
		NonAddShortestPath* getNonAddShPath();

		/** @return pointer to object that implements single-objective label-correcting
				algorithm.
		*/
		LabelCorrectingAl* getLabelCorrectingAlgo();
		/** Allocates appropriate DescDirectionPath object and returns a pointer to it. 
		*/
		DescDirectionPath* allocateDescDirectionPath(PathAlgoType algo, PathApp app, 
													 int nbLinks, FPType delta, 
													 FPType slope, FPType scaleFact);

		/** @return pointer to object that implements point-to-point bi-objective shortest
				path algorithm.
		*/
		Point2PointBiObjShPath* getPoint2PointBSP();
		/** @return pointer to object responsible for travel time and toll lower bounds calculation.
		*/
		BoundsCalculatorForBSPBase* getBoundsCaclulator();
		/** @return pointer to object responsible for addition of known paths to the destination node.
		*/
		PathsAdder* getPathsAdder();
		/** @return pointer to object that implements non-additive path cost calculation.
		*/
		NonAdditivePC* getNonAdditivePC();
		/** @return pointer to object that implements one-source bi-objective shortest path algorithm.
		*/
		OneSourceBiObjShPath* getOneSourceBSP();
		/** Generates random tolls on shortest paths and writes them to file.
		*/
		void generateRndTollsAndWriteToFile(const std::string& rndTollsFile);
		/** @return pointer to object responsible for travel time and toll lower bound calculation
				for the reversed network.
		*/
		BoundsCalculatorForBackwardNet* getBackwardBound();
		/** @return pointer to object that implements path cost domination rule.
		*/
		DominationByPathCostBase* getDominationByPathCost();
};

#endif
