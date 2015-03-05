#include "Astar.h"
#include "PathSet.h"
#include "ODMatrix.h"
#include "StarNetwork.h"
#include "Origin.h"
#include "ODSet.h"
#include "StarNode.h"

#include <set>
#include <algorithm>
#include <sstream>
#include <limits>

void Astar::initNodes(){
    static const FPType FPType_Max = std::numeric_limits<FPType>::max();
    std::fill(LabelVector->begin(), LabelVector->end(), FPType_Max);
    std::fill(Predecessors->begin(), Predecessors->end(),  -1);
}

FPType Astar::getCost(int destIndex) const{
    assert((destIndex >= 0) && (destIndex < nbNodes_));
    return (*LabelVector)[destIndex];
}

StarLink* Astar::getInComeLink(int destIndex) const{
    assert((destIndex >= 0) && (destIndex < nbNodes_));
    int linkIndex = (*Predecessors)[destIndex];
    return  linkIndex == -1 ?  NULL :  netPointer_->getLink(linkIndex);
}

Astar::Astar(StarNetwork* netPointer, ODMatrix* odMatrix) : ShortestPath(), 
            bestKnownPathCost_(std::numeric_limits<FPType>::infinity()),
            pathSet_(NULL) {

    netPointer_ = netPointer;
    nbNodes_ = netPointer_->getNbNodes();

    Queue = new PriorityQueue();
    LabelVector = new std::vector<FPType>(nbNodes_);
    Predecessors = new std::vector<int>(nbNodes_);

    nbDests_ = 0;

    netPointer_->calculateLinkCosts();

    // we need to calc the zero-flow travel times from every node to the destinations,
    // we use the following code since total number of origins = total number of destinations

}

void Astar::initializeBounds(ODMatrix* odMatrix) {
    // all the destinations
    std::set<int> destinations;

    for (OriginIterator it = odMatrix->begin(); it != odMatrix->end(); ++it){
        Origin* origin = *it;
        for (PairODIterator jt = origin->begin(); jt != origin->end(); ++jt) {
            PairOD* od = *jt;
            destinations.insert(od->getIndex());
        }
    }

    // last element of set is the largest, which is the largest we have to allocate for
    nbDests_ = 1+*destinations.rbegin();

    zeroFlowTimes = new std::vector<FPType>(nbNodes_*nbDests_); 

    for(int i = 0; i < nbNodes_; i++){
        calculate(i);
        for (std::set<int>::iterator it=destinations.begin(); it!=destinations.end(); ++it){
            zeroFlowTimes->at(i*nbDests_ + *it) = LabelVector->at(*it);
        }
    }
}

Astar::~Astar(){
    delete Queue;
    delete zeroFlowTimes;
    delete LabelVector;
    delete Predecessors;
}

void Astar::calculate(int O) {


    int u, v;
    FPType Duv;
    StarLink *nextLink;
    StarNode *curNode;

    PriorityQueue &Q = *Queue;
    std::vector<FPType> &L = *LabelVector;
    std::vector<int> &P = *Predecessors;
    StarNetwork &NP = *netPointer_;

    initNodes();

    Q = PriorityQueue();

    L[O] = 0;
    Q.push(PQPair(0, O));

    while ( !Q.empty() ){

        FPType Du = Q.top().first;
        u = Q.top().second;
        Q.pop();

        curNode = NP.beginNode(u);

        if (curNode == NULL)
            continue;

        if (shouldSkipZone(curNode, u, O)) //(curNode->getIsZone() && u != O)
            continue;

        for (nextLink = NP.beginLink();
                nextLink != NULL;
                nextLink = NP.getNextLink()) {

            v = nextLink->getNodeToIndex();

            Duv = Du + nextLink->getTime();

            if ( Duv < L[v] ){
                L[v] = Duv;
                P[v] = nextLink->getIndex();
                Q.push(PQPair(Duv, v));
            }

        } 
    } 

}

void Astar::calculate(int O, int D, int odPairIndex) {

    updateBestKnownPathCost(odPairIndex);

    int u, v;
    FPType Duv;
    StarLink *nextLink;
    StarNode *curNode;

    PriorityQueue &Q = *Queue;
    std::vector<FPType> &L = *LabelVector;
    std::vector<FPType> &H = *zeroFlowTimes;
    std::vector<int> &P = *Predecessors;
    StarNetwork &NP = *netPointer_;

    initNodes();
    Q = PriorityQueue();

    L[O] = 0;
    Q.push(PQPair(0, O));
    while ( !Q.empty() ){
        
        u = Q.top().second;
        FPType Du = L[u];
        Q.pop();

        if ( u == D ){ 
            break;
        }

        curNode = NP.beginNode(u);

        if (curNode == NULL)
            continue;

        if (shouldSkipZone(curNode, u, O))
            continue;

        for (nextLink = NP.beginLink();
                nextLink != NULL;
                nextLink = NP.getNextLink()) {

            v = nextLink->getNodeToIndex();

            Duv = Du + nextLink->getTime();

            if ( Duv < L[v] ){
                L[v] = Duv;
                P[v] = nextLink->getIndex();
                FPType estimatedPathCost = Duv + H[v*nbDests_ + D];
                if (estimatedPathCost < bestKnownPathCost_) {
                    Q.push(PQPair(estimatedPathCost, v));
                    if (v == D) {
                        bestKnownPathCost_ = estimatedPathCost;
                    }
                }
            }

        }
    }
}

bool Astar::shouldSkipZone(StarNode* node, int nodeIndex, int originIndex) const {
    return (node->getIsZone() && nodeIndex != originIndex);
};

void Astar::setPathSet(PathSet* pathSet) {
    assert(pathSet != NULL);
    pathSet_ = pathSet;
};

void Astar::updateBestKnownPathCost(int odPairIndex) {
    ODSet* odSet = pathSet_->getODSetByIndex(odPairIndex);
    bestKnownPathCost_ = odSet->getMinPathCost();
};
