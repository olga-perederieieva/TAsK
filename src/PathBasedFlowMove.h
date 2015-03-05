#ifndef PATH_BASED_FLOW_MOVE
#define PATH_BASED_FLOW_MOVE

#include <list>

class PathAndDirection;
class DescDirectionPath;
class Path;

/** \brief This class is responsible for performing a flow move within current ODSet.
	\note Only path flows are changed (link flows remain the same).
*/
class PathBasedFlowMove {
	public:
		virtual ~PathBasedFlowMove(){};
		
		/** Calculates path direction of descent.
			@return true if it is impossible to move flow (it happens only in case of
			PathBasedFlowMoveWithStep when calculated step size is zero), false otherwise.
		*/
		bool calculatePathDirection(const std::list<Path*> &paths);
		
		/** This method is responsible for shifting path flows according to a specified algorithm.
		*/
		virtual bool executeFlowMove() = 0;
		
		/** @return pointer to the first PathAndDirection object in the list.
			\warning it is not safe to use this method in a nested loop when this method is 
			called more than once on the same object.
		*/
		PathAndDirection* beginPathDirection() const;
		/** @return pointer to the next PathAndDirection object in the list, NULL if end of list.
			\warning it is not safe to use this method in a nested loop when it is 
			called more than once on the same object.
		*/
		PathAndDirection* getNextPathDirection() const;

	protected:
		
		PathBasedFlowMove(DescDirectionPath* pathDirectionAlgo);
		
		PathAndDirection** pathDirection_;
		int size_;
		mutable int currDirection_;
		
	private:
		
		DescDirectionPath* pathDirectionAlgo_; 
};

#endif
