#ifndef ADD_HOOK_STORE_OUT
#define ADD_HOOK_STORE_OUT

#include "AddHookScreenOut.h"
#include "FileWriter.h"

#include <vector>
#include <sstream>
#include <iomanip>

/** \brief This class stores convergence information into internal data structure and is able to 
	print it to the file. For file format see printToFile(). 
*/
class AddHookStoreOut : public AddHook { 
		public:
			AddHookStoreOut(){};
			virtual ~AddHookStoreOut(){};
			
			/** Stores convergence data into internal data structure.
				@param timePassed time passed since last measurement.
				@param gap current value of convergence measure.
			*/
			virtual void produceOut(FPType timePassed, FPType gap) {
				times_.push_back(timePassed);
				gaps_.push_back(gap);
			};
			
			/** Prints stored data to file specified in \b fileName. 
				File format is the following: {CPU time in seconds} {RGAP}\n */
			void printToFile(const std::string& fileName) {
				std::cout << "Writing convergence to file: " << fileName << std::endl;
				FileWriter wfile(fileName);
				int size = gaps_.size();
				std::string line("");
				for (int i = 0; i < size; ++i){
					std::ostringstream strs;
					strs << std::setprecision(5)  << times_[i] << " " << std::setprecision(16) << gaps_[i] << "\n";
					line = strs.str();
					wfile.writeLine(line);
				}
			};
			
		private:
			std::vector<FPType> times_; /**< stored values of calculation times.*/
			std::vector<FPType> gaps_; /**< stored values of convergence measurements.*/
};

#endif
