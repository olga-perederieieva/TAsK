#ifndef ORIGIN_H
#define ORIGIN_H

#include <list>

class PairOD;

typedef std::list<PairOD*>::iterator PairODIterator;

/** \brief This class is used to represent the container of destination nodes corresponding
 *  to one origin.
 */
class Origin {
	
	public:
		
		/** @param index origin index.
		*/
		explicit Origin(int index);

		/** This class is responsible for deleting all destinations once it is destroyed.
		*/
		~Origin();
		
		/** Adds destination dest to the set of destinations belonging to this origin.
		*/
		void addDestination(PairOD *dest);
		
		/** @return iterator pointing to the first destination belonging to this origin.
		*/
		PairODIterator begin();
		/** @return end of container.
		*/
		PairODIterator end();
		
		/**	@return origin index.
		*/
		int getIndex() const;
		/** @return true if there is no destinations belonging to this origin, false otherwise.
		*/
		bool isEmpty() const;
		/**	@return total number of destination nodes belonging to this origin.
		*/
		int getNbDest() const;
	
	private:
	
		const int index_;
		std::list<PairOD*> destList_;	
};

#endif
