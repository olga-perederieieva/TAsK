#ifndef ONE_SOURCE_BI_OBJ_SH_PATH
#define ONE_SOURCE_BI_OBJ_SH_PATH 

#include "BiObjLabelContainer.h"

/** \brief Abstract base class that defines interface for all one-source
	bi-objective shortest path algorithms.
*/
class OneSourceBiObjShPath {
	public:
		
		virtual ~OneSourceBiObjShPath() {};
		/** Calculates bi-objective shortest paths.
			@param originIndex origin index.
		*/
		virtual void calculate(int originIndex) = 0;
		/** @return reference to container of non-dominated labels.
		*/
		const BiObjLabelContainer& getLabels() const {
			return labels_;
		};

	protected:
		BiObjLabelContainer labels_;
		/** @param nbNodes total number of nodes in the network.
		*/
		OneSourceBiObjShPath(int nbNodes) : labels_(nbNodes) {};

};

#endif