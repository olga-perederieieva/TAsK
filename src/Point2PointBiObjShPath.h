#ifndef POINT_2_POINT_BI_OBJ_SH_PATH
#define POINT_2_POINT_BI_OBJ_SH_PATH 

#include "BiObjLabelContainer.h"

#include <list>

class BiObjLabel;
class StarLink;

/** \brief Defines interface for point-to-point bi-objective shortest path algorithms. 
*/
class Point2PointBiObjShPath {
	public:
		
		virtual ~Point2PointBiObjShPath() {};
		/** Solves point-to-point bi-objective shortest path problem.
			@param originIndex origin index.
			@param destIndex destination index.
			@param odIndex O-D pair index.
		*/
		virtual void calculate(int originIndex, int destIndex, int odIndex) = 0;
		/** Backtracks the path from a given label labelWithMinCost and
			stores path in path.
		*/
		virtual void createPath(BiObjLabel *labelWithMinCost, std::list<StarLink*>& path) = 0;
		/** @return reference to the container with non-dominated labels.
		*/
		const BiObjLabelContainer& getLabels(){
			return labels_;
		};

	protected:
		BiObjLabelContainer labels_;
		Point2PointBiObjShPath(int nbNodes) : labels_(nbNodes) {};

};

#endif