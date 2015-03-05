class BiObjLabel;

/** \brief Implements comparison operator for sorting bi-objective labels
	lexicographically with respect to travel time as the first objective
	and toll as the second objective.
*/
class LexOptComparison {
	public:
		bool operator() (BiObjLabel* lhs, BiObjLabel* rhs) const;
};

