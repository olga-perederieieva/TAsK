#ifndef PARAMS_
#define PARAMS_

#include <string>
#include <map>

/** \brief This class is used for storing parameters loaded from *.params or text file.
	\details format of *.params file is the following: \<Attribute name\> : {Attribute value}.
	Comments of the following format are allowed \b/\b/ or \b/\b* \b*\b/. For description of existing 
	attributes see file <a href="../../input.params">input.params</a> and 
	<a href="index.html">Main Page</a>.
*/ 
class Params {
	public:
		Params();
		~Params();
		
		/** @return value of a given attribute \b name in a string format. 
			If the given attribute does not exist, an error is thrown.
		*/
		std::string getParam(const std::string &name);
		
		/** Same as getParam(), but the returned string is without any white spaces.
		*/
		std::string getParamWoSpaces(const std::string &name);

		/** Saves a given attribute and its value in internal data structure.
			All attributes are assumed to be unique. If an existing attribute is attempted to
			be added, an error is thrown.
		*/
		void addParameter(const std::string &field, const std::string &value);

		/** Prints all attributes and their corresponding values on screen.
		*/
		void print();
		
		/** Generates auto-name for a file using the following format: 
			NetName_Algorithm_Equilibration_Approach_LineSearch_NEWTON_STEPS_ShPathAlgo
			+ bi-objective shortest paths parameters if any.
			The default delimiter is _.
			If one of the attributes is not given, they will be skipped.
		*/
		std::string getAutoFileName(char del = '_');
		
	private:
		std::map<std::string, std::string> paramDict_;
};

#endif
