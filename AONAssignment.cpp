#include "AONAssignment.h"
#include "ODMatrix.h"

AONAssignment::AONAssignment(const ODMatrix &mat, ShortestPath* shPath) :
				shPath_(shPath), mat_(mat){ 

};

AONAssignment::~AONAssignment(){

};

FPType AONAssignment::execute(InitialiseSolution *solution){
	FPType minTravelTime = 0.0;
	for (ConstOriginIterator it = mat_.begin(); it != mat_.end(); ++it){
		minTravelTime += performAssignmentForOrigin(solution, *it);
	}
	return minTravelTime;
};
