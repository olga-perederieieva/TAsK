#include <stdlib.h>
#include <iostream>

#include "Params.h"
#include "StepSizeAdd.h"
#include "ObjectManager.h"
#include "Error.h"
#include "FWAlgo.h"
#include "RelGap.h"
#include "Bisection.h"
#include "Armijo.h"
#include "QuadApp.h"
#include "PathBasedAlgo.h"
#include "AdditivePC.h"
#include "NonAdditivePC.h"
#include "LinkFlowsCFW.h"
#include "LinkFlowsBFW.h"
#include "OriginBasedAlgo.h"
#include "OriginBasedAlgoTapas.h"
#include "OriginBasedAlgoEQII.h"
#include "Astar.h"
#include "AddHookScreenAndStore.h"
#include "AlgoDecorator.h"
#include "AlgoDecoratorWriteLinks.h"
#include "PathBasedFlowMovePE.h"
#include "PASManagerWithStep.h"
#include "PathBasedAlgoEqII.h"
#include "DescDirectionPathScaled.h"
#include "DescDirectionPathPE.h"
#include "DescDirectionPathGP.h"
#include "DescDirectionPathPG.h"
#include "DescDirectionPathISP.h"
#include "OriginBasedAlgoTapasEqII.h"
#include "AONNonAdditive.h"
#include "MaxDiffConvMeasure.h"
#include "NonAddParser.h"
#include "TollsManagement.h"
#include "DefaultNetParser.h"
#include "DefaultODMatrixParser.h"
#include "LabelSettingOneSourceBSP.h"
#include "LabelSettingPoint2PointBSP.h"
#include "BoundsCalculatorWithTimeUpdate.h"
#include "BiDirectionalPoint2PointBSP.h"
#include "ParseIndiffCurves.h"
#include "NoPathsAdder.h"
#include "CurrentPathsAdder.h"
#include "ShortestPathAdder.h"
#include "Astar.h"
#include "ParseSpecificIndiffCurves.h"
#include "IndiffCurveContainerLinearCurve.h"
#include "IndiffCurveContainerConvexCurve.h"
#include "IndiffCurveContainerConcaveCurve.h"
#include "BoundsCalculatorForBackwardNet.h"
#include "DominationByPathCostBase.h"
#include "DominationByPathCost.h"
#include "NonAddShortestPathWithLazyBSP.h"
#include "ShortestPathWithRandomReturn.h"
#include "NonAddShortestPathForAON.h"
#include "LazyShortestPath.h"
#include "AstarWithoutZoneSkip.h"
#include "ShortestPathWithRandomReturnWithFixedProbability.h"
#include "OriginSet.h"
#include "PathBasedFlowMoveGP.h"
#include "PathBasedFlowMoveWithStep.h"
#include "PathSet.h"
#include "LinkFncContainer.h"
#include "Derivative.h"
#include "ODMatrix.h"

ObjectManager::ObjectManager(Params *params): params_(params) {
	
	isAdditive_ = -1;
	
	net_ = NULL;
	odMatrix_ = NULL;
	algo_ = NULL;
	linkFnc_ = NULL;
	linkFlows_ = NULL;
	lineSearch_ = NULL;
	convMeasure_ = NULL;
	shPath_ = NULL;
	der_ = NULL;
	pathSet_ = NULL;
	pathCost_ = NULL;
	algoType_ = Nothing;
	algoApp_ = AppNothing;
	originSet_ = NULL;
	addHook_ = NULL;
	addHookStore_ = NULL;
	pasManager_ = NULL;
	descDirPath_ = NULL;
	flowMove_ = NULL;
	stepSize_ = NULL;
	stepSizeAdd_ = NULL;
	flowMoveWithStep_ = NULL;
	flowMoveGP_ = NULL;
	LCShortestPath_ = NULL;
	nonAddShPath_ = NULL;
	ndPathCost_ = NULL;
	indiffCurveCont_ = NULL;
	tolls_ = NULL;
	oneSourceBSP_ = NULL;
	point2pointBSP_ = NULL;
	boundsCalculator_ = NULL;
	pathsAdder_ = NULL;
	currPathsAdder_ = NULL;
	aStar_ = NULL;
	backwardBounds_ = NULL;
	dominationRule_ = NULL;
	aStarForNonAdd_ = NULL;
	boundsWithTimeUpdate_ = NULL;
};

ObjectManager::~ObjectManager(){
	
	
	delete net_;
	delete odMatrix_;
	delete algo_;
	delete linkFnc_;
	delete linkFlows_;
	delete lineSearch_;
	delete convMeasure_;
	if (shPath_ == LCShortestPath_ || shPath_ == nonAddShPath_ || shPath_ == aStar_) {
		shPath_ = NULL;
	} else {
		delete shPath_;
	}
	delete der_;
	delete pathSet_;
	if (pathCost_ == ndPathCost_) {
		pathCost_ = NULL;
	} else {
		delete pathCost_;
	}
	delete originSet_;
	delete addHook_;
	delete pasManager_;
	delete descDirPath_;
	delete flowMove_;
	delete stepSize_;

	delete LCShortestPath_;
	delete nonAddShPath_;
	delete ndPathCost_;
	delete indiffCurveCont_;
	delete tolls_;
	delete oneSourceBSP_;
	delete point2pointBSP_;
	delete boundsCalculator_;
	delete pathsAdder_;
	delete aStar_;
	delete backwardBounds_;
	delete dominationRule_;
};

PathCost* ObjectManager::getPathCost(){
	if (pathCost_ == NULL){
		if (getIfAdditive()) {
			std::cout << "Creating additive path cost" << std::endl;
			pathCost_ = new AdditivePC();
		} else {
			std::cout << "Creating non-additive path cost" << std::endl;
			getNonAdditivePC();
		}
	}
	return pathCost_; 
};

NonAdditivePC* ObjectManager::getNonAdditivePC(){
	assert(getIfAdditive() == false);
	if (ndPathCost_ == NULL) {
		std::cout << "Creating non-additive PC" << std::endl;
		assert(pathCost_ == NULL);
		ndPathCost_ = new NonAdditivePC(*getIndiffCurveContainer(), *getTolls());
		pathCost_ = ndPathCost_;
		std::cout << "Non-additive PC created" << std::endl;
	}
	return ndPathCost_;
};

IndiffCurveContainer* createProperIndiffCurveContainer(const std::string& curveType, 
			int nbOdPairs){
	if (curveType == "PiecewiseLinear") {
		return new IndiffCurveContainer(nbOdPairs);
	} else if (curveType == "Linear") {
		return new IndiffCurveContainerLinearCurve(nbOdPairs);
	} else if (curveType == "Convex") {
		return new IndiffCurveContainerConvexCurve(nbOdPairs);
	} else if (curveType == "Concave") {
		return new IndiffCurveContainerConcaveCurve(nbOdPairs);
	} else {
		throw Error("Unexpected value of parameter <INDIFF_CURVE_TYPE>");
	}
	return NULL;
};

IndiffCurveContainer* ObjectManager::getIndiffCurveContainer(){
	assert(getIfAdditive() == false);
	if (indiffCurveCont_ == NULL) {
		std::cout << "Creating indiffCurveCont" << std::endl;
		std::string curves =  params_->getParam("INDIFF_CURVE");
		std::string curveType =  params_->getParamWoSpaces("INDIFF_CURVE_TYPE");
		indiffCurveCont_ = createProperIndiffCurveContainer(curveType,
				getODMatrix()->getNbODPairs());
		if (curves == "RND_GEN") {
			indiffCurveCont_->generateRandomCurvesWithBSP(static_cast<int>(
				getFloatValue("MAX_NB_POINTS_PER_CURVE")),
				*getOneSourceBSP(),
				*getODMatrix()	);
			indiffCurveCont_->writeToFile(getNet()->getNetName() + ".curves", 
				*getODMatrix()  );
		} else {
			
			if (curveType == "PiecewiseLinear") {
				ParseIndiffCurves curvesParser;
				curvesParser.parse(curves, *indiffCurveCont_);
			} else if (curveType == "Linear" || curveType == "Convex" 
				|| curveType == "Concave") {
				ParseSpecificIndiffCurves curvesParser;
				curvesParser.parse(curves, *indiffCurveCont_);
			} else {
				throw Error("Unexpected value of parameter <INDIFF_CURVE_TYPE>");
			}
		}
		std::cout << "IndiffCurveCont created" << std::endl;
	}
	return indiffCurveCont_;
};

TollContainerType* ObjectManager::getTolls(){
	assert(getIfAdditive() == false);
	if (tolls_ == NULL) {
		std::cout << "Creating tolls" << std::endl;
		getNet();
		std::cout << "Tolls created" << std::endl;
	}
	return tolls_;
};

bool ObjectManager::getIfAdditive(){
	if ( isAdditive_ == -1) {
		std::string tmp =  params_->getParamWoSpaces("INDIFF_CURVE");
		 isAdditive_ = tmp.empty();
	}
	return  isAdditive_;
};

FPType ObjectManager::getFloatValue(std::string paramName){
	if (paramName == "") return 0.0;
	FPType value = 0.0;
	try {
		std::string tmp =  params_->getParamWoSpaces(paramName);
		if (tmp.empty()) {
			throw Error("Parameter <" + paramName + "> was not initialised");
		}
		value = atof(tmp.c_str());
	} catch (...) {
		throw Error("Unexpected value of parameter <" + paramName + ">");
	}
	return value;
};

void ObjectManager::generateRndTollsAndWriteToFile(const std::string& rndTollsFile) {
	
	LabelCorrectingAl shPath(net_);
	TollsManagement::generateRandomTollsOnShortestPath(*tolls_, 
		getFloatValue("RND_TOLLS_PROBABILITY"), 
		static_cast<TollType>(getFloatValue("RND_TOLLS_MAX_TOLL")), 
		getODMatrix(), &shPath);
	int nbTolledLinks = 0;
	for (int i = 0; i < net_->getNbLinks(); ++i)
		if ((*tolls_)[i] != 0.0) ++nbTolledLinks;
	std::cout << "nbTolledLinks = " << nbTolledLinks << std::endl;
	TollsManagement::writeTollsToFile(*tolls_,  params_->getParam("NETWORK"), 
			rndTollsFile, getNet());
};

StarNetwork* ObjectManager::getNet(){
	if (net_ == NULL) {
		std::cout << "Creating network" << std::endl;
		if (getIfAdditive()) {
			DefaultNetParser parser( params_->getParam("NETWORK"));
			net_ = parser.parseNetwork();
			net_->linkNodes();
			std::cout << "DefaultNetParser and link nodes finished" << std::endl;
		} else {
		
			NonAddParser parser( params_->getParam("NETWORK"));
			
			net_ = parser.parseNetwork();
			net_->linkNodes();
			tolls_ = parser.getTolls();
			
			std::string rndTollsFile =  params_->getParamWoSpaces("WRITE_RND_TOLLS_TO_FILE");
			if (!rndTollsFile.empty()) {
				generateRndTollsAndWriteToFile(rndTollsFile);
			}
			
			assert(tolls_ != NULL);
		}
		std::cout << "Network created " << std::endl;
	}
	return net_;
};

ODMatrix* ObjectManager::getODMatrix(){
	if (odMatrix_ == NULL) {
		std::cout << "Creating OD matrix" << std::endl;
		std::string file =  params_->getParam("OD_MATRIX"); 
		DefaultODMatrixParser parser(file);
		odMatrix_ = parser.parseODMatrix(getNet());
		std::cout << "OD matrix created" << std::endl;
	}
	return odMatrix_;
};

ShortestPath* ObjectManager::getShPath(){
	if (shPath_ == NULL) {
		std::cout << "Creating shortest path" << std::endl;
		std::string algo =  params_->getParamWoSpaces("ShPathAlgo");
	    if( algo == "LC"){
	    	assert(getIfAdditive() == true);
	       	LCShortestPath_ = new LabelCorrectingAl(getNet());
	       	shPath_ = LCShortestPath_;
	    } else if( algo == "Astar"){
	    	assert(getIfAdditive() == true);
        	aStar_ = new Astar(getNet(), getODMatrix());
        	aStar_->initializeBounds(getODMatrix());
        	shPath_ = aStar_;
        } else if (algo == "NonAdd" || algo == "LazyNonAdd" || algo == "NonAddWithAstar") {
        	assert(getIfAdditive() == false);
        	nonAddShPath_ = getNonAddShPath();
        } else if (algo == "LazySP") {
        	assert(getIfAdditive() == true);
        	shPath_ = new LazyShortestPath(new LabelCorrectingAl(getNet()));
        } else {
        	throw Error("Unexpected parameter for <ShPathAlgo>. Possible values are LC, Astar.");
        }
       
	    if (nonAddShPath_ == NULL && ! params_->getParamWoSpaces("UseP2PShPathWithRandomReturn").empty()) {
			std::cout << " params_->getParamWoSpaces(UseP2PShPathWithRandomReturn) = " <<
       	 	 params_->getParamWoSpaces("UseP2PShPathWithRandomReturn") << std::endl;
       	 	if (! params_->getParamWoSpaces("FIXED_PROBABILITY").empty()) {
       	 		FPType probability = getFloatValue("FIXED_PROBABILITY");
       	 		if (probability <= 0 || probability > 1) throw Error("Unexpected value of parameter <FIXED_PROBABILITY>");
       	 		shPath_ = new ShortestPathWithRandomReturnWithFixedProbability(shPath_, probability);
       	 	} else {
				shPath_ = new ShortestPathWithRandomReturn(shPath_);
			}
		}
	    std::cout << "Shortest path created" << std::endl;
	}
	return shPath_;
};

Derivative* ObjectManager::getDerivative(){
	if (der_ == NULL){
		std::cout << "Creating derivative" << std::endl;
		if (getIfAdditive() == true) {
			der_ = new Derivative(getFloatValue("ZERO_FLOW"), getLinkFncCont());
		} else {
			throw Error("Derivative is not implemented for the non-additive case");
		}
		std::cout << "Derivative created" << std::endl;
	}	
	return der_;
};

LineSearch* ObjectManager::getLineSearch(){
	if (lineSearch_ == NULL) {
		if (getIfAdditive() == false) {
			throw Error("Line search is not implemented for the non-additive case");
		}
		// line search, possible values: BISEC, ARMIJO, QUAD_APP
		std::cout << "Creating line search" << std::endl;
		std::string tmp =  params_->getParamWoSpaces("LINE_SEARCH");
		if (tmp == "BISEC") {
			FPType pr = getFloatValue("LS_PRECISION");
			if (pr <= 0.0) throw Error("Line search precision must be positive");
			lineSearch_ = new Bisection(pr, getDerivative());
		} else if (tmp == "ARMIJO") {
			FPType dec = getFloatValue("ARMIJO_DEC");
			if (dec <= 0.0) throw Error("Decrement for Armijo search must be positive");
			lineSearch_ = new Armijo(dec, getDerivative());
		} else if (tmp == "QUAD_APP") {
			lineSearch_ = new QuadApp(getDerivative(), getNet());
		} else {
			throw Error("Unexpected value of parameter <LINE_SEARCH>");
		}
		std::cout << "Line search created" << std::endl;
		
	}
	return lineSearch_;
};

ConvMeasure* ObjectManager::getConvMeasure(){
	if (convMeasure_ == NULL) {
		std::string conv =  params_->getParamWoSpaces("CONV_MEASURE");
		if (conv == "RGAP") {
			assert(getIfAdditive() == true);
			std::cout << "Creating relative gap" << std::endl;
			convMeasure_ = new RelGap(getFloatValue("PRECISION"), getNet(), getODMatrix(), getLabelCorrectingAlgo());
			std::cout << "RGAP created" << std::endl;
		} else if (conv == "MAX_DIFF") {
			assert(getPathAlgoType() != Nothing);
			convMeasure_ = new MaxDiffConvMeasure(getFloatValue("PRECISION"), getShPath(),
							getODMatrix(), getPathSet());
		} else {
			throw Error("Unexpected convergence measure");
		}
	}
	return convMeasure_;
};

PathAlgoType ObjectManager::getPathAlgoType(){
	if ( algoType_ == Nothing) {
		std::string tmp =  params_->getParamWoSpaces("ALGORITHM");
		if (tmp == "PE") {
			 algoType_ = PE;
		} else if (tmp == "GP") {
			 algoType_ = GP;
		} else if (tmp == "PG") {
			 algoType_ = PG;
		} else if (tmp == "ISP"){
			 algoType_ = ISP;
		}
	}
	return  algoType_;
};

PathApp ObjectManager::getPathAlgoApp(){
	if (algoApp_ == AppNothing) {
		std::string tmp =  params_->getParamWoSpaces("APPROACH");
		if (tmp == "APP1") {
			algoApp_ = APP1;
		} else if (tmp == "APP2") {
			algoApp_ = APP2;
		} else if (tmp == "APP3") {
			algoApp_ = APP3;
		} else if (tmp == "") {
			algoApp_ = APP1; 
		} else {
			throw Error("Unexpected approach type");
		}
	}
	return algoApp_;
};

DescDirectionPath* ObjectManager::getDescDirectionPath(){
	if (descDirPath_ == NULL) {
		std::cout << "Creating Desc direction" << std::endl;
		descDirPath_ = allocateDescDirectionPath(getPathAlgoType(), getPathAlgoApp(), 
						getNet()->getNbLinks(), getFloatValue("DIR_TOLERANCE"), 
						getFloatValue("SLOPE"), 
						getFloatValue("ISP_SCALE"));
		std::cout << "descDirection created" << std::endl;
	}
	return descDirPath_;
};

PathSet* ObjectManager::getPathSet(){
	if (pathSet_ == NULL) {
		std::cout << "Creating path set" << std::endl;
		PathApp app = getPathAlgoApp();
		
		AONAssignment* aon = NULL;
		if (getIfAdditive()) {
			aon = new AONUsual(*getODMatrix(), getLabelCorrectingAlgo());
		} else {
			aon = new AONNonAdditive(*getODMatrix(), new NonAddShortestPathForAON(getNonAdditivePC(), 
							getNet()->getNbNodes(),
            				getOneSourceBSP(), getODMatrix()));
		}

		if (app == APP3) {
			PathAlgoType algo = getPathAlgoType();
			if (algo == PE ) {
				pathSet_ = PathSet::createSetPEAPP3(getNet(), getODMatrix(), getShPath(), 
							getPathCost(), 
							getFloatValue("ZERO_FLOW"), getPathBasedFlowMove(), 
							aon);
			} else if (algo == GP) {
				pathSet_ = PathSet::createSetGPAPP3(getNet(), getODMatrix(), getShPath(), 
							getPathCost(), 
							getFloatValue("ZERO_FLOW"), getPathBasedFlowMoveGP(), 
							aon);
			} else {
				throw Error("Approach 3 is not implemented for this algorithm");
			}
		} else {
			pathSet_ = PathSet::createSetWithStep(getNet(), getODMatrix(), getShPath(), 
						getPathCost(), 
						getFloatValue("ZERO_FLOW"), getPathBasedFlowMoveWithStep(), 
						aon);
		}
		std::cout << "Path set created" << std::endl;
	}
	return pathSet_;
};
		
DecoratedEqAlgo* ObjectManager::getEqAlgo(const std::string& dirConv, 
							const std::string& dirFlows){
	if (algo_ == NULL) {
		std::cout << "Creating algo" << std::endl;
		std::string tmp =  params_->getParamWoSpaces("ALGORITHM");
		if ((tmp == "FW") || (tmp == "CFW") || (tmp == "BFW")) {
			assert(getIfAdditive() == true);
			if (linkFlows_ == NULL) {
				if (tmp == "CFW") {
					linkFlows_ = new LinkFlowsCFW(getNet(), getODMatrix(), getShPath(), getLineSearch(), 
						getFloatValue("ZERO_FLOW"), getLabelCorrectingAlgo());
				} else if (tmp == "BFW") {
					linkFlows_ = new LinkFlowsBFW(getNet(), getODMatrix(), getShPath(), getLineSearch(),
						getFloatValue("ZERO_FLOW"), getLabelCorrectingAlgo());
				} else {
					linkFlows_ = new LinkFlows(getNet(), getODMatrix(), getShPath(), getLineSearch(),
						getFloatValue("ZERO_FLOW"), getLabelCorrectingAlgo());
				}
	
			}
			algo_ = new FWAlgo(linkFlows_, getAddHook(), getNet(), getConvMeasure(), 
					getFloatValue("TIME_LIMIT")); 
		} else if (getPathAlgoType() != Nothing){
			std::string tmp =  params_->getParamWoSpaces("EQUILIBRATION");
			std::cout << "EQ = " << tmp << std::endl;

			if (tmp == "EQI") {
				
				algo_ = new PathBasedAlgo(getPathSet(), getAddHook(), getConvMeasure(), getNet(),
					getFloatValue("TIME_LIMIT"), getODMatrix()); 
			} else if (tmp == "EQII") {
				algo_ = new PathBasedAlgoEqII(getPathSet(), getAddHook(), getConvMeasure(), getNet(),
					getFloatValue("TIME_LIMIT"), 
					static_cast<int>(getFloatValue("MAX_ITER")), getODMatrix());
			} else {
				throw Error("Unexpected value of parameter <EQUILIBRATION>");
			}
			if (currPathsAdder_ != NULL) currPathsAdder_->setPathSet(getPathSet());
			if (aStar_ != NULL) aStar_->setPathSet(getPathSet());
			if (aStarForNonAdd_ != NULL) aStarForNonAdd_->setPathSet(getPathSet());

		} else if (tmp == "B" || tmp == "Bstep") {
			assert(getIfAdditive() == true);
			std::string tmp =  params_->getParamWoSpaces("EQUILIBRATION");
			std::cout << "EQ = " << tmp << std::endl;
			if (tmp == "EQI") {
				algo_ = new OriginBasedAlgo(getOriginSet(), getNet(), getAddHook(),  
							getConvMeasure(), getFloatValue("TIME_LIMIT"));
			} else if (tmp == "EQII") {
				algo_ = new OriginBasedAlgoEQII(getOriginSet(), getNet(), getAddHook(),
						getConvMeasure(), getFloatValue("TIME_LIMIT"), 
						static_cast<int>(getFloatValue("MAX_ITER")));
			} else {
				throw Error("Unexpected value of parameter <EQUILIBRATION>");
			}
		} else if (tmp == "LUCE") {
			assert(getIfAdditive() == true);
			std::string tmp =  params_->getParamWoSpaces("EQUILIBRATION");
			std::cout << "EQ = " << tmp << std::endl;
			if (tmp == "EQI") {
				algo_ = new OriginBasedAlgo(getOriginSet(), getNet(), getAddHook(),
					getConvMeasure(), getFloatValue("TIME_LIMIT"));
			} else if (tmp == "EQII") {
				algo_ = new OriginBasedAlgoEQII(getOriginSet(), getNet(), getAddHook(),
						getConvMeasure(), getFloatValue("TIME_LIMIT"), 
						static_cast<int>(getFloatValue("MAX_ITER")));
			} else {
				throw Error("Unexpected value of parameter <EQUILIBRATION>");
			}
		} else if (tmp == "TAPAS" || tmp == "TAPASstep") {
			assert(getIfAdditive() == true);
			std::string tmp =  params_->getParamWoSpaces("EQUILIBRATION");
			std::cout << "EQ = " << tmp << std::endl;
			if (tmp == "EQI") {
				algo_ = new OriginBasedAlgoTapas(getOriginSet(), getNet(), getPASManager(), 
					getAddHook(), getConvMeasure(), getFloatValue("TIME_LIMIT"));
			} else if (tmp == "EQII") {
				algo_ = new OriginBasedAlgoTapasEqII(getOriginSet(), getNet(), getPASManager(), 
					getAddHook(), getConvMeasure(), getFloatValue("TIME_LIMIT"),
					static_cast<int>(getFloatValue("MAX_ITER")));
			} else {
				throw Error("Unexpected value of parameter <EQUILIBRATION>");
			}

		} else {
			throw Error("Unexpected value of parameter <ALGORITHM>");
		}
		
		std::cout << "Adding decorations" << std::endl;
		tmp =  params_->getParamWoSpaces("CONVERGENCE");
		if (tmp != "") {
			getAddHook();
			assert(addHookStore_ != NULL);
			if (tmp == "AUTO") {
				tmp =  params_->getAutoFileName() + ".conv";
			}
			algo_ = new AlgoDecorator(algo_, addHookStore_, tmp, dirConv);
		}
		tmp =  params_->getParamWoSpaces("LINK_FLOWS");
		if (tmp != "") {
			if (tmp == "AUTO") {
				tmp =  params_->getAutoFileName() + ".flows";
			}
			algo_ = new AlgoDecoratorWriteLinks(algo_, getNet(), tmp, dirFlows);
		}
		std::cout << "Algo created" << std::endl;
	}
	return algo_;
};

LinkFncContainer* ObjectManager::getLinkFncCont(){
	if (linkFnc_ == NULL) {
		std::cout << "Creating linkFncContainer" << std::endl;
		linkFnc_ = new LinkFncContainer(getNet());
		std::cout << "linkFncContainer created" << std::endl;
	}
	return linkFnc_;
};

OriginSet* ObjectManager::getOriginSet(){
	assert(getIfAdditive() == true);
	if (originSet_ == NULL) {
		std::cout << "Creating origin set" << std::endl;
		std::string tmp =  params_->getParamWoSpaces("ALGORITHM");
		
		if (tmp == "B") {

			std::string steps =  params_->getParamWoSpaces("NEWTON_STEPS");
			bool useMulti = true;
			if (steps == "SINGLE") {
				useMulti = false;
			} else if (steps == "MULTI") {
				useMulti = true;
			} else {
				throw Error("Unexpected value of parameter <NEWTON_STEPS>");
			} 

			originSet_ = OriginSet::createOriginSetB(getODMatrix(), getNet(), 
					getFloatValue("ZERO_FLOW"), getFloatValue("DIR_TOLERANCE"), useMulti, 
					getLabelCorrectingAlgo());
		} else if (tmp == "Bstep"){
			originSet_ = OriginSet::createOriginSetBWithStep(getODMatrix(), getNet(), 
					getFloatValue("ZERO_FLOW"), getFloatValue("DIR_TOLERANCE"), 
					getLineSearch(), getLabelCorrectingAlgo());
		} else if (tmp == "LUCE") {
			originSet_ = OriginSet::createOriginSetLUCE(getODMatrix(), getNet(), getFloatValue("ZERO_FLOW"), 
						getFloatValue("DIR_TOLERANCE"), getLineSearch(), getLabelCorrectingAlgo());
		} else if (tmp == "TAPAS" || tmp == "TAPASstep") {
			originSet_ = OriginSet::createOriginSetTAPAS(getODMatrix(), getNet(), getFloatValue("ZERO_FLOW"), 
						getFloatValue("DIR_TOLERANCE"), getPASManager(), getLabelCorrectingAlgo());
		} else {
			throw Error("Unexpected algorithm value");
		}
		std::cout << "origin set created" << std::endl;
	}
	return originSet_;
};

AddHook* ObjectManager::getAddHook(){
	if (addHook_ == NULL) {
		std::cout << "Creating AddHook" << std::endl;
		std::string onScreen =  params_->getParamWoSpaces("SHOW_CONVERGENCE");
		std::string toFile =  params_->getParamWoSpaces("CONVERGENCE");
		if (onScreen != "") {
			if (toFile != "") {
				addHookStore_ = new AddHookScreenAndStore();
				addHook_ = addHookStore_;
			} else {
				addHook_ = new AddHookScreenOut();
			}
		} else if (toFile != "") {
			addHookStore_ = new AddHookStoreOut();
			addHook_ = addHookStore_;
		} else {
			addHook_ = new AddHook();
		}
		std::cout << "AddHook created" << std::endl;	
	}
	return addHook_;
};


PASManager* ObjectManager::getPASManager(){
	assert(getIfAdditive() == true);
	if (pasManager_ == NULL) {
		std::cout << "Creating PASManager" << std::endl;
		std::string tmp =  params_->getParamWoSpaces("ALGORITHM");
		if (tmp == "TAPAS") {
			pasManager_ = new PASManager(*getShPath(), getFloatValue("DIR_TOLERANCE"), getNet()->getNbLinks(), 
						getFloatValue("MU"), getFloatValue("V"), 
						getFloatValue("ZERO_FLOW"));
		} else if (tmp == "TAPASstep") {
			pasManager_ = new PASManagerWithStep(*getShPath(), getFloatValue("DIR_TOLERANCE"), getNet()->getNbLinks(), 
						getFloatValue("MU"), getFloatValue("V"), getNet()->getNbLinks(),
						getLineSearch(), getFloatValue("ZERO_FLOW"));
		} else {
			throw Error("Unexpected algorithm type. Possible values are TAPAS or TAPASwithStep");
		}
		std::cout << "PASManager created" << std::endl;
	}
	return pasManager_;
};

PathBasedFlowMove* ObjectManager::getPathBasedFlowMove(){
	if (flowMove_ == NULL) {
		std::cout << "Creating PathBasedFlowMove" << std::endl;
		PathAlgoType algo = getPathAlgoType();
		assert(algo != Nothing);
		PathApp app = getPathAlgoApp();
		if (app == APP3) {
			if (algo == PE) {
				flowMove_ = new PathBasedFlowMovePE(getDescDirectionPath());
			} else if (algo == GP) {
				flowMoveGP_ = new PathBasedFlowMoveGP(getFloatValue("ALPHA"), getDescDirectionPath()); 
				flowMove_ = flowMoveGP_;
			} else {
				throw Error("Approach 3 is not implemented for this algorithm");
			}
		} else {
			flowMoveWithStep_ = new PathBasedFlowMoveWithStep(getStepSize(), getDescDirectionPath(), 
				getFloatValue("ZERO_FLOW"));
			flowMove_ = flowMoveWithStep_;
		}
		std::cout << "PathBasedFlowMove created" << std::endl;
	}	
	return flowMove_;
};

StepSize* ObjectManager::getStepSize(){
	if (stepSize_ == NULL) {
		std::cout << "Creating StepSize" << std::endl;
		if (getIfAdditive() == true) {
			std::string tmp =  params_->getParamWoSpaces("LINE_SEARCH");
			stepSizeAdd_ = new StepSizeAdd(getLineSearch(), getDerivative()); // so far only additive step size 
			stepSize_ = stepSizeAdd_;
		} else {
			throw Error("Step size is not implemented for the non-additive case");
		}
		std::cout << "StepSize created" << std::endl;
	} 
	return stepSize_;
};

StepSizeAdd* ObjectManager::getStepSizeAdd(){
	assert(getIfAdditive() == true);
	if (stepSizeAdd_ == NULL) {
		std::cout << "Creating StepSizeAdd" << std::endl;
		stepSizeAdd_ = new StepSizeAdd(getLineSearch(), getDerivative()); // so far only additive step size 
		stepSize_ = stepSizeAdd_;
		std::cout << "StepSizeAdd created" << std::endl;
	} 
	return stepSizeAdd_;
};

PathBasedFlowMoveWithStep* ObjectManager::getPathBasedFlowMoveWithStep(){
	assert(getIfAdditive() == true);
	if (flowMoveWithStep_ == NULL) {
		std::cout << "Creating PathBasedFlowMoveWithStep" << std::endl;
		flowMoveWithStep_ = new PathBasedFlowMoveWithStep(getStepSize(), getDescDirectionPath(), 
			getFloatValue("ZERO_FLOW"));
		flowMove_ = flowMoveWithStep_;
		std::cout << "PathBasedFlowMoveWithStep created" << std::endl;
	}
	return flowMoveWithStep_;
};

PathBasedFlowMoveGP* ObjectManager::getPathBasedFlowMoveGP(){
	if (flowMoveGP_ == NULL) {
		std::cout << "Creating PathBasedFlowMoveGP" << std::endl;
		flowMoveGP_ = new PathBasedFlowMoveGP(getFloatValue("ALPHA"), getDescDirectionPath()); 
		flowMove_ = flowMoveGP_;
		std::cout << "PathBasedFlowMoveGP created" << std::endl;
	}
	return flowMoveGP_;
};


LabelCorrectingAl* ObjectManager::getLabelCorrectingAlgo(){
	assert(getIfAdditive() == true);
	if (LCShortestPath_ == NULL) {
		std::string algo =  params_->getParamWoSpaces("ShPathAlgo");
		LCShortestPath_ = new LabelCorrectingAl(getNet());
		if (shPath_ == NULL && algo == "LC") {
			shPath_ = LCShortestPath_;
		} 
	}
	return LCShortestPath_;
};

NonAddShortestPath* ObjectManager::getNonAddShPath(){
	assert(getIfAdditive() == false);
	if (nonAddShPath_ == NULL) {
		std::cout << "Creating non-additive shortest path" << std::endl;
		std::string algo =  params_->getParamWoSpaces("ShPathAlgo");
		if (algo == "LazyNonAdd") {
			nonAddShPath_ = new NonAddShortestPathWithLazyBSP(getNonAdditivePC(), getNet()->getNbNodes(),
            				getOneSourceBSP(), getODMatrix()); 
		} else if (algo == "NonAdd") {
			nonAddShPath_ = new NonAddShortestPath(getNonAdditivePC(), getNet()->getNbNodes(),
            				getOneSourceBSP(), getPoint2PointBSP(), getODMatrix());
			
		} else {
			throw Error("Unexpected value of parameter <ShPathAlgo>");
		}
		if (shPath_ == NULL && (algo == "NonAdd" || algo == "LazyNonAdd" || algo == "NonAddWithAstar")) {
			shPath_ = nonAddShPath_;
		} 
		if (! params_->getParamWoSpaces("UseP2PShPathWithRandomReturn").empty()) {
			std::cout << "******************************************" << std::endl;
			if (! params_->getParamWoSpaces("FIXED_PROBABILITY").empty()) {
	       	 	FPType probability = getFloatValue("FIXED_PROBABILITY");
	       	 	if (probability <= 0 || probability > 1) throw Error("Unexpected value of parameter <FIXED_PROBABILITY>");
	       	 		shPath_ = new ShortestPathWithRandomReturnWithFixedProbability(shPath_, probability);
	       	 	} else {
					shPath_ = new ShortestPathWithRandomReturn(shPath_);
				}

			std::cout << "******************************************: " << 
			(shPath_ == nonAddShPath_) << std::endl;
		}
		std::cout << "Non-additive shortest path created" << std::endl;
	}
	return nonAddShPath_;
};

DescDirectionPath* ObjectManager::allocateDescDirectionPath(PathAlgoType algo, PathApp app, int nbLinks, 
				FPType delta, FPType slope, FPType scaleFact){
	assert(delta > 0.0);
	assert(slope > 0.0);
	assert(scaleFact > 0.0);
	if (app == APP3) {
		if (algo == PE) {
			return new DescDirectionPathScaled(new DescDirectionPathPE(delta), nbLinks, delta);
		} else if (algo == GP){
			return new DescDirectionPathScaled(new DescDirectionPathGP(delta), nbLinks, delta);
		} else {
			throw Error("Approach 3 is not defined for this algorithm type");
		}
	} else if (app == APP2){
		if (algo == GP) {
			return new DescDirectionPathScaled(new DescDirectionPathGP(delta), nbLinks, delta);
		} else if (algo == PE) {
			return new DescDirectionPathScaled(new DescDirectionPathPE(delta), nbLinks, delta);
		} else {
			throw Error("Approach 2 is not defined for this algorithm type");
		}
	} else if (app == APP1) {
		if (algo == PE) {
			return new DescDirectionPathPE(delta);
		} else if (algo == GP) {
			return new DescDirectionPathGP(delta);
		} else if (algo == PG) {
			return new DescDirectionPathPG(delta);
		} else if (algo == ISP) {
			return new DescDirectionPathISP(slope, scaleFact, delta);
		} else {
			throw Error("Path-based algorithm is not defined");
		}
	} else {
		throw Error("Path-based algorithm is not defined");
	}
	return NULL;
};

OneSourceBiObjShPath* ObjectManager::getOneSourceBSP() {
	assert(getIfAdditive() == false);
	if (oneSourceBSP_ == NULL) {
		std::cout << "Creating OneSourceBSP" << std::endl;
		oneSourceBSP_ = new LabelSettingOneSourceBSP(*getNet(), *getTolls()); 
		std::cout << "OneSourceBSP created" << std::endl;
	}
	return oneSourceBSP_;
};

Point2PointBiObjShPath* ObjectManager::getPoint2PointBSP(){
	assert(getIfAdditive() == false);
	if (point2pointBSP_ == NULL) {
		std::cout << "Creating p2pBSP" << std::endl;
		std::string bsp =  params_->getParamWoSpaces("BIOBJ_SHPATH_P2P");
		if (bsp == "BiLabelSetting") {
			point2pointBSP_ = new LabelSettingPoint2PointBSP(*getNet(), *getTolls(), *getBoundsCaclulator(),
				*getPathsAdder(), *getDominationByPathCost());
		
		} else if (bsp == "BiLabelSetting_bidirectional"){
			point2pointBSP_ = new BiDirectionalPoint2PointBSP(*getNet(), *getTolls(),
						*getBoundsCaclulator(),
						*getPathsAdder(), *getBackwardBound(), *getDominationByPathCost());
		} else {
			throw Error("Unexpected value of parameter <BIOBJ_SHPATH_P2P>.");
		}
		std::cout << "P2PBSP created" << std::endl;
	}
	return point2pointBSP_;
};

BoundsCalculatorForBSPBase* ObjectManager::getBoundsCaclulator() {
	assert(getIfAdditive() == false);
	if (boundsCalculator_ == NULL) {
		std::cout << "Creating BoundsCalculatorForBSP" << std::endl;
		std::string bsp =  params_->getParamWoSpaces("BLS_BOUNDS");
		if (bsp.empty()) {
			boundsCalculator_ = new BoundsCalculatorForBSPBase;
		} else if (bsp == "zeroFlow") {
			boundsCalculator_ = new BoundsCalculatorForBSP(*getNet(), *getTolls());
		} else if (bsp == "currentFlow") {
			boundsWithTimeUpdate_ = new BoundsCalculatorWithTimeUpdate(*getNet(), *getTolls());
			boundsCalculator_ = boundsWithTimeUpdate_;
		} else {
			throw Error("Unexpected value of parameter <BIOBJ_SHPATH_P2P>.");
		}
		std::cout << "BoundsCalculatorForBSP created" << std::endl;
		boundsCalculator_->initializeBounds();
	}
	return boundsCalculator_;
};

PathsAdder* ObjectManager::getPathsAdder() {
	assert(getIfAdditive() == false);
	if (pathsAdder_ == NULL) {
		std::cout << "Creating PathAdder" << std::endl;
		std::string usePaths =  params_->getParamWoSpaces("USE_EXISTING_PATHS");
		if (usePaths.empty()) {
			pathsAdder_ = new NoPathsAdder();
		} else if (usePaths == "onePath") {
			currPathsAdder_ = new ShortestPathAdder(*getTolls());
			pathsAdder_ = currPathsAdder_;
		} else if (usePaths == "currentPaths") {
			currPathsAdder_ = new CurrentPathsAdder(*getTolls());
			pathsAdder_ = currPathsAdder_;
		} else {
			throw Error("Unexpected value of parameter <USE_EXISTING_PATHS>.");
		}
		std::cout << "PathAdder created" << std::endl;
	}
	return pathsAdder_;
};

BoundsCalculatorForBackwardNet* ObjectManager::getBackwardBound() {
	if (backwardBounds_ == NULL) {
		std::cout << "creating backward bounds *********************" << std::endl;
		backwardBounds_ = new BoundsCalculatorForBackwardNet(*getNet(), *getTolls());

		backwardBounds_->initializeBounds();
	}
	return backwardBounds_;

};

DominationByPathCostBase* ObjectManager::getDominationByPathCost() {
	if (dominationRule_ == NULL) {
		std::string usePathCost =  params_->getParam("USE_PATH_COST_DOMINATION");
		if (usePathCost.empty()) {
			dominationRule_ = new DominationByPathCostBase;
		} else {
			std::cout << "creating path cost domination rule" << std::endl;
			dominationRule_ = new  DominationByPathCost(*getNonAdditivePC());
			std::cout << "path cost domination rule created" << std::endl;
		}
	}
	return dominationRule_;
};