#ifndef OD_PAIR_
#define OD_PAIR_

#include "UsedTypes.h"

/** \brief This class implements a destination. It is a basic element of Origin class.
*/
class PairOD {
	
	public:
		/** @param index destination index.
			@param demand O-D demand.
		*/
		PairOD(int index, FPType demand); 
		~PairOD();
		
		/** Prints all relevant information on screen.
		*/
		void print();
		
		/** @return demand.
		*/
		FPType getDemand() const;
		/** @return destination index.
		*/
		int getIndex() const;
		/** @return O-D pair index.
		*/
		int getODIndex() const;
		/** Sets O-D pair index to index.
		*/
		void setODIndex(int index);
	
	private:
	
		const int index_;
		const FPType demand_; 
		int odIndex_;
};

#endif
