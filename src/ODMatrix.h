#ifndef OD_MATRIX
#define OD_MATRIX

#include "UsedTypes.h"

#include <string>
#include <tr1/unordered_map>
#include <vector>

class Origin;

typedef std::vector<Origin*>::iterator OriginIterator;
typedef std::vector<Origin*>::const_iterator ConstOriginIterator;

/** \brief This class implements origin-destination matrix.
*/
class ODMatrix {
	public:
		
		ODMatrix(int nbNodes, int nbOrigins);
		~ODMatrix();
		
		/** This method is used for creation of the matrix.
			Adds Origin object  to O-D matrix.
		*/
		void addOrigin(Origin *origin);
		
		/** @return iterator pointing to the beginning of he container.
		*/
		OriginIterator begin();

		/** @return end of the container.
		*/
		OriginIterator end();

		ConstOriginIterator begin() const;
		ConstOriginIterator end() const;
		
		/** This method creates O-D indexes for all O-D pairs and fills the data structure
			used to support getDemandByIndex() method. 
			\note It MUST be called once when add origins have been added to the class.
		*/
		void setIndexes();
		
		/** Prints O-D matrix on the screen.
		*/
		void print();
		
		/** @return total number of O-D pairs.
		*/
		int getNbODPairs() const;
		/** @return total number of origins.
		*/
		int getNbOrigins() const;
		
		/** This method returns demand of node \b destIndex coming from origin \b originIndex.
			\note Complexity in average case: constant; worst case: linear in container size.
			For details see http://www.cplusplus.com/reference/unordered_map/unordered_map/find/.
		*/
		FPType getDemandByIndex(int originIndex, int destIndex) const;

		/** @return pointer to the Origin object given origin index.
		*/
		Origin* getOriginByIndex(int index) const;
		
	private:
	
		const int nbNodes_; 
		int nbODPairs_;
		std::vector<Origin*> odMatrix_; /**< internal representation of O-D matrix */
		std::tr1::unordered_map<std::string, FPType> demandByIndex_; /**< special structure that allows to */
																	/**< get demand of node given origin and destination */
																	/**< indexes */	
		inline std::string createKey(int originIndex, int destIndex) const;	
};

#endif
