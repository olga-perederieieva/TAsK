#ifndef ORIGIN_SET
#define ORIGIN_SET

#include "InitialiseSolution.h"
#include "AONBushBased.h"

class OriginBush;
class ODMatrix;
class StarNetwork;
class PASManager;
class LineSearch;
class LabelCorrectingAl;

#include <vector>

typedef std::vector<OriginBush*>::iterator OriginSetIterator;

/** \brief This class represents a container for OriginBush objects.
	\details It is also responsible for instantiation of correct objects.
*/
class OriginSet : public InitialiseSolution {
	
	public:
		/** This factory method must be used for algorithm B.
		*/
		static OriginSet* createOriginSetB(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, bool useMultiStep, LabelCorrectingAl* LCShPath);
		
		/** This factory method must be used for algorithm B with line search.
		*/  
		static OriginSet* createOriginSetBWithStep(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, LineSearch* lineSearch, 
					LabelCorrectingAl* LCShPath);

		/** This factory method must be used for LUCE.
		*/
		static OriginSet* createOriginSetLUCE(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, LineSearch* lineSearch, LabelCorrectingAl* LCShPath);
		
		/** This factory method must be used for TAPAS.
		*/
		static OriginSet* createOriginSetTAPAS(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, 
					FPType dirTol, PASManager *pasManager, LabelCorrectingAl* LCShPath);

		~OriginSet();
		
		/** Initialises all bushes by using AON procedure.
		*/
		void initialise();
		

		/** @return iterator that points to the first bush in container.
		*/
		OriginSetIterator begin();
		/** @return end of container.
		*/
		OriginSetIterator end();
		
		void initialiseItself(StarLink* link, PairOD *dest, int originIndex);
		
		/** Prints bushes on the screen.
		*/
		void print();
		
	private:
		StarNetwork *net_;
		const int nbOrigins_;
		std::vector<OriginBush*> bushes_;
		std::vector<int> indexes_;
		AONBushBased aon_;
		
		OriginSet(ODMatrix *mat, StarNetwork *net, FPType zeroFlow, FPType dirTol,
					PASManager *pasManager, 
					LabelCorrectingAl* LCShPath, bool useMultiStep, 
					LineSearch* lineSearch, OriginAlgoType algo);
			
};

#endif
