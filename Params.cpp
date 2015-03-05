// #include <stdlib.h>
#include <iostream>

#include "Params.h"
#include "Error.h"
#include "UtilsString.h"

Params::Params(){
	
};

Params::~Params(){

};
 
std::string Params::getParam(const std::string &name){
	std::map<std::string, std::string>::iterator it = paramDict_.find(name);
	if (it == paramDict_.end()) {
		throw Error("Unexpected parameter name: " + name);
	}
	return it->second;
};

std::string Params::getParamWoSpaces(const std::string &name){
	return Utils::deleteWhiteSpaces(getParam(name));
};

void Params::addParameter(const std::string &field, const std::string &value){
	if ((paramDict_.insert(std::pair<std::string, std::string>(field, value))).second == false ) {
		std::string message = "Field: ";
		message.append(field);
		message.append(" was already added with value: ");
		message.append(value);
		Error er(message);
		throw er;
	}
};

void Params::print(){
	std::map<std::string, std::string>::iterator it = paramDict_.begin();
	for(; it != paramDict_.end(); ++it) {
		std::cout << "Field: " << it->first << " Value: " << it->second << std::endl;
	}
};

std::string Params::getAutoFileName(char del){
	std::string netName;
	Utils::extractName(getParam("NETWORK").c_str(), netName);
	std::string attributes[] = {"ALGORITHM", "EQUILIBRATION", "APPROACH", "LINE_SEARCH", 
			"NEWTON_STEPS", "ShPathAlgo", "BIOBJ_SHPATH_P2P", "BLS_BOUNDS", "USE_EXISTING_PATHS", 
			"ART2_TOLLS", "INDIFF_CURVE_TYPE"};
	int nbAtt = sizeof(attributes) / sizeof(std::string);
	std::string attributeValue("");
	std::string retVal(netName + del);
	for (int i = 0; i < nbAtt; ++i) {
		try {
			attributeValue = getParamWoSpaces(attributes[i]);
		} catch (Error e){
			attributeValue = "";
		}
		if (attributeValue == "yes") attributeValue = "withPaths";
		if (attributeValue != "") retVal.append(attributeValue + del);
	}
	if (!getParamWoSpaces("USE_PATH_COST_DOMINATION").empty()) retVal.append("pathCostDomination");
	if (!getParamWoSpaces("UseP2PShPathWithRandomReturn").empty()) retVal.append("_rndReturn");
	std::string prob = getParamWoSpaces("FIXED_PROBABILITY");
	if (!prob.empty()) retVal.append("_prob_" + prob);
	return retVal;
};
