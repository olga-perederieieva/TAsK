#ifndef _ASTAR_
#define _ASTAR_

#include <vector>
#include <queue>
#include <limits>
#include <cassert>

#include "ShortestPath.h"

class StarNetwork;
class ODMatrix;
class PathSet;
class StarNode;

/** \brief This class implements A* shortest path for point-to-point case,
    and Dijkstra for origin case. 
    \author Boshen Chen
*/
class Astar : public ShortestPath {
    private:

        typedef std::pair<FPType, int> PQPair;

        class Prioritize{
            public:
                bool operator() ( const PQPair& p1, const PQPair& p2){
                    return p1.first > p2.first;
                }
        };

        typedef std::priority_queue<PQPair, std::vector<PQPair>, Prioritize > PriorityQueue;

        PriorityQueue *Queue;

        std::vector<FPType> *zeroFlowTimes;
        std::vector<FPType> *LabelVector;
        std::vector<int> *Predecessors;

        int nbDests_;
        int nbNodes_;

        StarNetwork *netPointer_;

        FPType bestKnownPathCost_;
        PathSet* pathSet_;
        
        void initNodes();

        void updateBestKnownPathCost(int odPairIndex);

    public:

        /** @param netPointer network.
            @param odMatrix O-D matrix.
        */
        Astar(StarNetwork* netPointer, ODMatrix* odMatrix);
        virtual ~Astar();

        /** Calculates zero flow bounds.
            @param odMatrix O-D matrix.
        */
        void initializeBounds(ODMatrix* odMatrix);

        void calculate(int O);
        void calculate(int O, int D, int odPairIndex);
        FPType getCost(int destIndex) const;
        StarLink* getInComeLink(int destIndex) const;

        /** Stores a pointer to the path set.
            \note Must be called once after object creation.
        */
        void setPathSet(PathSet* pathSet);

    protected:
        /** @return true if zone centroid must be skipped, false otherwise.
        */
        virtual bool shouldSkipZone(StarNode* node, int nodeIndex, int originIndex) const;

};

#endif
