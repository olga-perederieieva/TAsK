#ifndef BI_OBJ_LABEL
#define BI_OBJ_LABEL

#include "UsedTypes.h"

#include <string>

class StarLink;

/** \brief Implements bi-objective label.

	Each label contains travel time, toll, node index
	where this label belongs to, link that points to this label,
	label from which current label is generated.
*/
class BiObjLabel {
	public:
		/**
			@param time travel time.
			@param toll travel toll.
			@param owner node index where current label belongs to.
			@param prevLabel label from which current label is generated.
			@param prevLink link that points to this label.
		*/
		BiObjLabel(FPType time, TollType toll, int owner, BiObjLabel* prevLabel, StarLink* prevLink);
		~BiObjLabel();

		FPType getTime() const;

		TollType getToll() const;

		BiObjLabel* getPrevLabel() const;
		int getOwner() const;

		StarLink* getPrevLink() const;

		std::string toString() const;

		void setTime(FPType timeVal);

	private:
		FPType time_;
		const TollType toll_;
		const int owner_;
		BiObjLabel* prevLabel_;
		StarLink* prevLink_;
};

#endif