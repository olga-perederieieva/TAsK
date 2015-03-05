#ifndef UTILS_FOR_DOMINATION
#define UTILS_FOR_DOMINATION 

#include "UsedTypes.h"

#include <list>

class BiObjLabelContainer;
class BiObjLabel;
class StarLink;

/** \brief Defines useful functions for dominance checks for bi-objective
	shortest path algorithms.
*/
namespace UtilsForDomination {

	extern const FPType tolerance_;
	/** @return true if label with cost vector [newTime, newToll] is dominated by at least one
			label in the destination node destIndex, false otherwise.
	*/
	bool isDominatedByLabelInDestNode(const BiObjLabelContainer& labels, int destIndex, 
			FPType newTime, TollType newToll);
	/** @return true if label first is dominated by label second, false otherwise.
	*/
	bool isDominated(BiObjLabel* first, BiObjLabel* second);
	/** @return true if label  with cost vector [newTime, newToll] is dominated by
			label second, false otherwise.
	*/
	bool isDominated(FPType newTime, TollType newToll, BiObjLabel* second);
	/** Backtracks path from destLabel. Link of the path are added to \b path.
	*/
	void createPathFromLabel(BiObjLabel* destLabel, std::list<StarLink*> &path);
};

#endif