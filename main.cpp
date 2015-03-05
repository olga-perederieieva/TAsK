/**
 	@mainpage  Traffic Assignment frameworK (TAsK)

 	The TAsK software implements several algorithms for solving the deterministic static traffic 
 	assignment problem with fixed demands, see \cite Sheffi1985, and the non-additive traffic 
 	assignment model, see \cite Larsson2004. 

 	All code is implemented in the C++ programming language.
 	This is open-source software distributed under the MIT license.

 	The software was tested only on Ubuntu (12.10, 13.10 and 14.04) operating system.

 	\author <a href = http://www.des.auckland.ac.nz/people/o-perederieieva>Olga Perederieieva</a>
	\author <a href = https://github.com/Boshen>Boshen Chen</a> (A* star shortest path algorithm)
	\author Benoit Mulocher (bi-directional bi-objective label-setting algorithm)

	\b Contributors

    - <a href = http://www.lancaster.ac.uk/lums/people/all/matthias-ehrgott/>Matthias Ehrgott</a>
    - <a href = https://unidirectory.auckland.ac.nz/profile/a-raith>Andrea Raith</a>
	- <a href = http://www.its.leeds.ac.uk/people/staff-profiles/judith-wang/>Judith Y.T. Wang</a>
	- <a href = http://www.rsm.nl/people/marie-schmidt/>Marie Schmidt</a>

	The style sheet for documentation comes from https://github.com/Velron/doxygen-bootstrapped.

	\version 1.0

	\copyright MIT license

 	## How to cite
 	In a paper, please, cite the following reference:

 	> Olga Perederieieva, Matthias Ehrgott, Andrea Raith, Judith Y.T. Wang, 
 	> A framework for and empirical study of algorithms for traffic assignment, 
 	> Computers & Operations Research, Volume 54, February 2015, Pages 90-107, 
 	> ISSN 0305-0548, http://dx.doi.org/10.1016/j.cor.2014.08.024.

 	In code documentation, include a reference to this website.
	
	## Features
	- 11 algorithms for additive traffic assignment that belong to link-, path- and bush-based
		methods, see \cite Perederieieva2015.
	- 2 algorithms for non-additive traffic assignment (path equilibration and gradient projection).
	- 2 equilibration strategies, see \cite Perederieieva2015.
	- 3 algorithms for finding a step size (bisection, Armijo-like rule and quadratic approximation).
	- Various options for direction of descent.
	- 3 flow update strategies for path-based algorithms.
	- Non-additive shortest path algorithm based on bi-objective label-setting with 
		speed-up techniques that exploit the traffic assignment structure.
	- Plain text and KML output of computation results.
 	
	## Installation instructions

	Requirements:
	- C++ compiler (the software was tested with gcc 4.8.2).
	- Boost library that can be downloaded from http://www.boost.org/.

	Instructions:
	- Download archive with source code.
	- Unzip.
	- Open terminal, go to the folder with source code and type: make.

	By default extended floating point precision is used (long double type).
	In order to use double type, comment line
	CPPFLAGS += -DUSE_EXTENDED_PRECISION in <a href="../../Makefile">Makefile</a> and
	recompile code if necessary by typing in terminal: make clean; make.

	\note If double type is used instead of long double, algorithms might fail to find highly precise
	solutions.
	
	## Usage

	In order to run the TAsK software one should create a text file with various run parameters first.

	### Example data

	The archive with source code includes the SiouxFalls data instance from 
	http://www.bgu.ac.il/~bargera/tntp/. This instance can be used for
	research purposes only.

	### How to run
	
	After compiling the source code, one can run it with one of the two included parameter files. 
	Example parameter files are  
	<a href="../../input.params">input.params</a> for conventional traffic assignment
	and <a href="../../inputNonAdd.params">inputNonAdd.params</a>
	for non-additive traffic assignment. Please, use these files as templates for creating
	other files with parameters. 
	\note If some of the fields are missing or invalid values are used, the program will
	terminate with an error message or failed assertion.

	\warning All field names and possible values are case-sensitive.

	In order to run code with <a href="../../input.params">input.params</a>:
	- Open terminal and go to the folder with source code.
	- Type in terminal: ./ta input.params.

	File <a href="../../input.params">input.params</a> allows to solve the additive
	traffic assignment problem with the Algorithm B (for details, see \cite Dial_2006) based 
	on quadratic approximation
	for the SiouxFalls instance.

	File <a href="../../inputNonAdd.params">inputNonAdd.params</a> allows to solve the non-additive
	traffic assignment problem with the path equilibration algorithm
	 (for details, see \cite Florian1995)
	based on Newton step
	for the SiouxFalls instance.

	### Parameter files

	A parameter file must contain the following fields (supported comments \b/\b* for
	multiple line comments, \b/\b/ for one line comments):

	\code{.cpp}	<NETWORK>: {Data/SiouxFalls_tolls.txt}
	\endcode
	%Path to file with network data
	
	\code{.cpp}
	<OD_MATRIX>: {Data/SiouxFalls_trips.txt}
	\endcode
	%Path to file with O-D matrix.
		
	\code{.cpp}
	<WRITE_RND_TOLLS_TO_FILE>: {}
	\endcode
	This field is used for the non-additive traffic assignment model.
	If this field is not empty, then instead of tolls specified in file with network,
    random ones will be used. 
    This field is ignored if additive traffic assignment problem is solved.

    \code{.cpp}
	<RND_TOLLS_PROBABILITY>: {}
	\endcode
	Probability to assign tolls to links.
	This field is ignored if option <WRITE_RND_TOLLS_TO_FILE> has empty value.

	\code{.cpp}
	<RND_TOLLS_MAX_TOLL>: {}
	\endcode
	Maximum value of toll for randomly generated tolls.
	This field is ignored if option <WRITE_RND_TOLLS_TO_FILE> has empty value.

	\code{.cpp}
	<INDIFF_CURVE_TYPE>: {PiecewiseLinear}
	\endcode
	Type of scalarization function. 
	This field is used for the non-additive traffic assignment model.
    This field is ignored if option <INDIFF_CURVE> has empty value.

    Possible values:
     - PiecewiseLinear.
     - Linear.
     - Convex.
	 - Concave.

	\code{.cpp}
	<INDIFF_CURVE>: {Curves/SiouxFalls_tolls.curves} 
	\endcode
	%Path to file with scalarization functions.
	This field is used for the non-additive traffic assignment model.
	If this field is empty, then the problem is considered to be additive.
	If this field is set to RND_GEN, then piecewise-linear scalarization functions will
	be generated randomly.
	
	\code{.cpp}
	<MAX_NB_POINTS_PER_CURVE>: {3}
	\endcode
	Maximum number of breakpoints per piecewise-linear function.
	This field is ignored if option <INDIFF_CURVE> has any value except {RND_GEN}.

	\code{.cpp}
	<NODES>: {}
	\endcode
	%Path to file with node coordinates. Specify this field if LaTeX output is necessary.

	\code{.cpp}
	<TIME_LIMIT> : {20000}
	\endcode
	Time limit of algorithm execution in seconds.

	\code{.cpp}
	<ShPathAlgo> : {LC}
	\endcode
	Shortest path algorithm.
    Possible values:
    	- LC - single-source label-setting algorithm. If this option is chosen
	   			 along with path-based algorithm, then single-source label-setting
	   			 will be used for point-to-point shortest path calculations
	   			 which is much slower than A*. 
	   			 Works only for additive traffic
	   			 assignment. If one tries to choose this option for non-additive 
	   			 traffic assignment, then the program will terminate with failed assertion.
	    - %Astar - A* point-to-point algorithm. If this option is specified,
	   			A* will be used for path-based algorithm, and one-source
	     			Dijkstra's algorithm for convergence measure. 
	     			If this option is specified along with a link- or bush-based algorithm,
	     			then for all one-source shortest path calculations Dijkstra's algorithm
	     			will be used.
	     			Works only for additive traffic
	    		    assignment. If one tries to choose this option for non-additive 
	   			traffic assignment, then the program will terminate with failed assertion.
	   			\warning %Astar option does not work with TAPAS. Never use them together.
	    - NonAdd - non-additive shortest path.
	    			Works only for non-additive traffic
	    		    assignment. If one tries to choose this option for additive 
	   			traffic assignment, then the program will terminate with failed assertion.
 	    - LazyNonAdd - lazy non-additive shortest path, see \cite Chen1998.
 	    			Works only for non-additive traffic
	    		    assignment. If one tries to choose this option for additive 
	   			traffic assignment, then the program will terminate with failed assertion.
 	    - LazySP - lazy additive shortest path, see \cite Chen1998. 
		 	    Works only for additive traffic assignment.
		 	    If one tries to choose this option for non-additive 
	   			traffic assignment, then the program will terminate with failed assertion.
	.

	\code{.cpp}
	<UseP2PShPathWithRandomReturn>: {} 
	\endcode
	If any non-empty value is specified, then randomised flow update strategy is used 
	where probability of calculating a point-to-point shortest path depends on the iteration number.
	This option works only with path-based algorithms and is ignored if an algorithm 
	from a different group is used.

	\code{.cpp}
	<FIXED_PROBABILITY>: {}
	\endcode
	Probability of calculating a point-to-point shortest path. Possible values: any real number in 
	interval (0, 1]. This option is ignored if field <UseP2PShPathWithRandomReturn> has
	empty value.

	\code{.cpp}
	<CONV_MEASURE>: {MAX_DIFF} 
	\endcode
	Convergence measure. 
	Possible values: 
		- RGAP - relative gap. This option works for additive traffic assignment only.
		- MAX_DIFF - max-min cost difference bound, see \cite Dial_2006. This option works for
			path-based algorithms only (for both additive and non-additive problems).

	\code{.cpp}
	<PRECISION>: {1e-5}
	\endcode
	Algorithm precision. For example, if <PRECISION>: {1e-5}, then the algorithm will terminate when
	convergence measure reaches any value strictly smaller than 1e-5. Possible values:
	any positive number.

	\code{.cpp}
	<ZERO_FLOW>: {1e-10}
	\endcode
	Link flow tolerance. For example, if <ZERO_FLOW>: {1e-10}, then link flow will be reset to zero if
	its value is less than or equal to 1e-10. Possible values:
	any positive number.

	\code{.cpp}
	<DIR_TOLERANCE>: {1e-10}
	\endcode
	Tolerance of descent direction of path- and bush-based algorithms.
	For example, if <DIR_TOLERANCE>: {1e-10}, then a component 
	of the direction of descent is set to zero if its value is less than 1e-10. Possible values:
	any positive number.

	\code{.cpp}
	<ALGORITHM>: {PE}
	\endcode
	Algorithm.
	Possible values:
		- FW - Frank-Wolfe, see \cite Frank1956.
		- CFW - conjugate Frank-Wolfe, see \cite Mitradjieva2013.
		- BFW - bi-conjugate Frank-Wolfe, see \cite Mitradjieva2013.
		- PE - path equilibration, see \cite Dafermos1968 and \cite Florian1995.
		- GP - gradient projection, see \cite Jayakrishnan1994 and \cite Chen1998.
		- PG - projected gradient, see \cite MichaelFlorian2009.
		- ISP - improved social pressure, see \cite Kumar2011.
		- B - Algorithm B based on Newton step, see \cite Dial_2006 and \cite Boyles2011.
		- Bstep - Algorithm B based on a line search, see \cite Dial_2006 and \cite Boyles2011.
		- LUCE - linear user cost equilibrium, see \cite Gentile2014.
		- TAPAS - traffic assignment by paired alternative segments based on Newton step,
					see \cite Hillel2010.
		- TAPASstep - traffic assignment by paired alternative segments 
					based on  a line search, see \cite Hillel2010.
	.
	\warning In the case of non-additive traffic assignment only PE and GP options are available with 
	<APPROACH> set to {APP3}.
	
	\code{.cpp}
 	<LINE_SEARCH>: {BISEC}.
 	\endcode
 	Line search. This field is used only for algorithms based on line search.
 	If a different algorithm is specified, this field is ignored.
 	Possible values: 
 	   - BISEC - bisection.
 	   - ARMIJO - Armijo-like rule.
 	   - QUAD_APP - quadratic approximation.

 	\code{.cpp}
	<LS_PRECISION>: {1e-15}
	\endcode
	Line search tolerance. This field is used only if one of the line searcher is used.

	\code{.cpp}
	<ARMIJO_DEC>: {2}
	\endcode
	Decrement in Armijo-like rule. This option is applicable only if <LINE_SEARCH>: {ARMIJO}.

	\code{.cpp}
	<EQUILIBRATION>: {EQI}
	\endcode
	Type of equilibration. Possible values: 
		- EQI - Equilibration I, see \cite Perederieieva2015.
		- EQII - Equilibration II, see \cite Perederieieva2015.

	\code{.cpp}
	<MAX_ITER>: {10}
	\endcode
	Maximum number of iterations for Equilibration II.

	\code{.cpp}
	<APPROACH>: {APP3}
	\endcode
	Type of the direction of descent for path-based algorithms. 
	Possible values depend on the algorithm.
		- For PE: 
			+ APP1 - path equilibration based on a line search.
			+ APP3 - path equilibration based on Newton step.

		- For GP:
			+ APP1 - gradient projection based on a line search.
			+ APP2 - gradient projection based on a line search with scaled direction of descent.
			+ APP3 - gradient projection without a line search.

		- For PG:
			+ APP1 - projected gradient based on a line search.

		- For ISP:
			+ APP1 - improved social pressure based on a line search.
			.
		.

	\code{.cpp}
	<ALPHA>: {0.25}
	\endcode
	Constant that is used in gradient projection without a line search, see \cite Jayakrishnan1994.
	Valid values: any positive real number. This value must be set to a positive number even if
	gradient projection is not used.
	
	\code{.cpp}
	<SLOPE>: {1e-15}
	\endcode
	Minimum slope of the derivative of link cost function used in improved social pressure.
	This value must be set to a positive number even if improved social pressure is not used.
					
	\code{.cpp}
	<ISP_SCALE>: {0.15}
	\endcode
	Another parameter of parameter the improved social pressure algorithm.
	This value must be set to a positive number even if improved social pressure is not used.

	\code{.cpp}
	<NEWTON_STEPS>: {SINGLE}
	\endcode
	Type of Newton step for Algorithm B and TAPAS.
	Possible values: 
		- SINGLE - performs one Newton step.
		- MULTI - performs several Newton steps.

	\code{.cpp}
	<MU>: {0.5}
	\endcode
	Constant used in TAPAS for determining cost-effective %PAS.

	\code{.cpp}
	<V>: {0.25}
	\endcode
	Constant used in TAPAS for determining flow-effective %PAS.

	\code{.cpp}				
	<BIOBJ_SHPATH_P2P>: {BiLabelSetting} 
	\endcode
	Bi-objective point-to-point shortest path algorithm.
	This field is used on for non-additive traffic assignment.
	Possible values: 
		- BiLabelSetting - bi-objective label-setting. 
		- BiLabelSetting_bidirectional - bi-directional bi-objective label-setting.

	\code{.cpp}
	<BLS_BOUNDS> : {zeroFlow}
	\endcode
	Travel time lower bound. This field is used only for non-additive traffic assignment.
	If empty, then no bounds are used, otherwise possible values: 
		- zeroFlow - travel time bounds based on zero flows.
		- currentFlow - travel time bounds based on current flows.

	\code{.cpp}
	<USE_EXISTING_PATHS> : {onePath}
	\endcode
	Addition of known paths technique. 
	This field is used only for non-additive traffic assignment.
	If empty no paths are added, otherwise possible values are:
		- onePath - only shortest path is added.
		- currentPaths - all existing paths are added.

	\code{.cpp}
	<USE_PATH_COST_DOMINATION> : {yes}
	\endcode
	%Path cost domination rule. This field is used only for non-additive traffic assignment.
	If empty usual domination rule is used, otherwise domination by path cost is used.

	\code{.cpp}
	<SHOW_CONVERGENCE>: {yes}
	\endcode
	If the value of this field is not empty, prints convergence on the screen.

	\code{.cpp}
	<LINK_FLOWS>: {}
	\endcode
	%Path to file where link flows must be written. If the field is empty, no file will be created.
	If the field is set to AUTO, automatically generated name will be used.

	\code{.cpp}
	<CONVERGENCE>: {}
	\endcode
	%Path to file where convergence must be written.  If the field is empty, no file will be created.
	If the field is set to AUTO, automatically generated name will be used.
	
	\code{.cpp}
	<PATH_SET>: {}
	\endcode
	%Path to file where path sets must be written. Applicable only to path-based algorithms.
	 If the field is empty, no file will be created.
	If the field is set to AUTO, automatically generated name will be used.
	
	\code{.cpp}
	<LATEX_OUTPUT>: {net.tex}
	\endcode

	%Path to file where latex output must be written. If this field is specified, then
	field <NODES> must be specified too.  If the field is empty, no file will be created.
	If the field is set to AUTO, automatically generated name will be used.
		
	\note For KML output one has to modify main.cpp and use classes
	KMLNetOutput, KMLSelectLink, KMLPlotTolls and DiffKMLNetOutput.

	## Existing problems of the current implementation
	 - Excessive usage of pointers where references are more appropriate.
	 - In a few places arrays are used where vectors would be better.
	 - ObjectManager is responsible for creation of ALL objects.
	 - StarNetwork and PathBasedFlowMove iterator-like methods are not safe to use in nested loops.
	
 */

#include "Error.h"
#include "DecoratedEqAlgo.h"
#include "Timer.h"
#include "ParseParams.h"
#include "ObjectManager.h"
#include "Utils.h"
#include "FileWriter.h"
#include "PathSetPrinterToFile.h"
#include "LatexNetOutput.h"

#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include <stdlib.h>

#define NB_INPUT_PARAMS 2

int main(int argc, char *argv[]) {
	try {
		// For registering total CPU time 
		Timer timer; // time is started being measured on object creation

		// Check if the number of input parameters is correct
		if (argc < NB_INPUT_PARAMS) {
			Error er("Number of input parameters is not correct.");
			throw er;
		}
		
		srand(time(NULL));

		std::string fileParams = argv[1];
		
		std::string dirConv = "";
		std::string dirFlows = "";
		std::string dirCounters = "";
		std::string dirPathSet  ="";
		if (argc >= 5) {
			dirConv = argv[2];
			dirFlows = argv[3];
			dirCounters = argv[4];
		}
		if (argc == 6) {
			dirPathSet = argv[5];
		}
		
		Params params;
		ParseParams parser;
		parser.execute(fileParams, &params);
		
		Timer objCreationTimer;
		ObjectManager manager(&params);

		DecoratedEqAlgo *algo = manager.getEqAlgo(dirConv, dirFlows);
		FPType objCreationTime = objCreationTimer.getTimePassed();
		
		Timer exeTimer;
		int nbIter = algo->execute();

		FPType executionTime = exeTimer.getTimePassed();
		
		FPType maxDiffTest = Utils::checkFeasibility(manager.getNet(), manager.getODMatrix());

		FPType timePassed = timer.getTimePassed(); 
		std::stringstream ss;
		ss << "<TotalTime> : {" << timePassed <<  "}; <ObjectCreationTime> : {" << objCreationTime << 
				"}; <InitialisationTime> : {" << algo->getInitialisationTime() <<
				"}; <ExecutionTime> : {" << executionTime << 
				"}; <ConvTime> : {" << algo->getTotalConvTime() <<
				"}; <PureIterationTime> : {" << algo->getTotalPureIterTime() <<
				"}; <NodeFlowFeasibilityMaxChange> : {" << maxDiffTest << 
				"}; <nbIter> : {" << nbIter << "};\n";
		std::cout << ss.str();

		if (params.getParam("PATH_SET") != "") {
			std::string filename = params.getParam("PATH_SET");
			if (filename == "AUTO") 
				filename = dirPathSet + params.getAutoFileName() + ".paths";
			PathSetPrinterToFile writePathSet(manager.getODMatrix()); //, *manager.getTolls());
			writePathSet.writePathSetToFile(manager.getPathSet(), filename);
		}

		if (params.getParam("LATEX_OUTPUT") != "") {
			std::cout << "Writing latex file to: " << params.getParam("LATEX_OUTPUT") <<
				std::endl;
			
			LatexNetOutput latex(manager.getNet());
			latex.printToLaTeX(params.getParam("NODES"), params.getParam("LATEX_OUTPUT"), 
								true, 0.3);
		}

	} catch (Error error) {
		std::cout << "Error has occured during execution: " << error.getMessage() << std::endl;
		return 1;
	}

	return 0;
};
