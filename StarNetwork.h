#ifndef STAR_NETWORK
#define STAR_NETWORK

#include "UsedTypes.h"

#include <string>
#include <tr1/unordered_map>
#include <limits>
#include <list>

class StarNode;
class StarLink;

/** \brief This class implements forward star graph representation.
	\details For details see \cite Sheffi1985. */
class StarNetwork {
	public:
		StarNetwork(int nbNodes, int nbLinks, std::string &netName);
		~StarNetwork();
		
		/** Adds node \b node to the internal data structure. 
			\note Node is assumed to have out-going links that must be added 
			after node addition, using method 
			addLink(). This method might throw an error.
		*/
		void addNode(StarNode *node);

		/** Adds link \b link to internal data structure. Links are associated with the last 
			added node. This method might throw an error.
		*/
		void addLink(StarLink *link);
		
		/** This method must be called once after all links and nodes were added to the network.
			It create internal indexes (all internal indexes follow the convention that it is a number
			between 0 and number of nodes, or number of links for node and link indexes respectively) 
			for all nodes and sets them appropriately.
		*/
		void linkNodes();
		
		/** @return name of the network.
		*/
		std::string getNetName() const;
		/** @return total number of nodes in the network.
		*/
		int getNbNodes() const;
		/** @return total number of links in the network.
		*/
		int getNbLinks() const;
		
		/** @return pointer to the first node. It must be used together with getNextNode() method.
			For example, the entire forward star network
			can be iterated as follows:
			@code for (StarNode* node = beginNode(); node != NULL; node = getNextNode()) {
			    for (StarLink* link = beginLink(); link != NULL; link = getNextLink()) {
					//do something with link and/or node
				}
			} @endcode 
			\warning it is not safe to use this method in a nested loop when this method is 
			called more than once on the same object.
		*/
		StarNode* beginNode() const;
		/** Starts iteration process beginning with a node specified in the input by 
			index \b index. It must be used together with getNextNode() method.
			\warning it is not safe to use this method in a nested loop when this method is 
			called more than once on the same object.
		*/
		StarNode* beginNode(int index) const; 
		/** \warning it is not safe to use this method in a nested loop when this method is 
			called more than once on the same object.
		*/
		StarNode* getNextNode() const;
		/** This method must be used together with getNextLink() method.
			\warning it is not safe to use this method in a nested loop when this method is 
			called more than once on the same object.
		*/
		StarLink* beginLink() const;
		/** \warning it is not safe to use this method in a nested loop when this method is 
			called more than once on the same object.
		*/
		StarLink* getNextLink() const;

		/** This method is used in order to iterate through all links of the network. 
			It must be used together with getNextOnlyLink() method.
			\warning it is not safe to use this method in a nested loop when this method is 
			called more than once on the same object.
		*/
		StarLink* beginOnlyLink() const;
		/** \warning it is not safe to use this method in a nested loop when this method is 
			called more than once on the same object.
		*/
		StarLink* getNextOnlyLink() const;
		
		/** @return a pointer to a link specified by index \b linkIndex.
		*/
		StarLink* getLink(int linkIndex) const;
		
		/** @return a pointer of type StarNode* to a node specified by index \b index. 
			However, nodes of this type are created only if they have out-going links. That's why
			only such node can be returned by this method. If another node is attempted to get by
			using this method, then NULL is returned. 
		*/
		StarNode* getNodeWithLinks(int index);
		
		/** @return internal index of a node specified by its ID 
		 	\note Avoid calling this function as much as possible.
		 	It has average case complexity: constant, 
		 	worst case complexity: linear in container size.
		*/
		int getNodeIndex(int id);
		
		/** Prints network on screen.
		*/
		void print();

		/** Writes link flows to file \b fileName in the following format: 
			nodeFrom nodeTo linkFlow travelTime 
			Default number of printed digits for linkFlow and travelTime is 16.
		*/
		void printToFile(const std::string& fileName, int precision = 16);
		
		/** Loads link flows from file specified by \b fileName (link travel times are recalculated). 
			File format is:  nodeFrom nodeTo linkFlow travelTime
			\note Link flows must correspond to the created network object, i.e.
			the one that was specified in file with parameters.
		*/
		void loadFromFile(const std::string& fileName);
		
		/** Goes through all links and evaluates all link cost functions.
		*/
		void calculateLinkCosts();
		
		/** @return node ID given its internal index.
			Complexity is linear in the container size (total number of nodes).
		*/
		int findID(int index) const;

	private:

		const std::string netName_;
		const int nbNodes_;
		const int nbLinks_;
		std::vector<StarNode*> nodes_;
		std::vector<StarLink*> links_;
		std::vector<int> pointers_;
		int size_;
		int sizeLinks_;
		mutable int curNode_;
		mutable int curLink_;
		mutable int curOnlyLink_;
		std::tr1::unordered_map<int, int> idMap_;
		bool linkAdded_;
	
		/** Creates table of mapping between node ID and node indexes.
		*/
		void createIndexes();
		/** This method is used in loadFromFile(). It attempts to load link flow \b\ flow to 
			a link with nodes 
			\b nodeFrom and \b nodeTo. If such link does not exists false is returned.
		*/
		bool assignLinkFlow(int nodeFrom, int nodeTo, FPType flow);
};

#endif
