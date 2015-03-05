#ifndef STAR_NODE
#define STAR_NODE

/** \brief This class is a basic element of StarNetwork. It represents a node with out-going links.
*/
class StarNode {
	public:
		/** @param id node ID.
			@param isZone true if node if a zone node, false otherwise
		*/
		StarNode(int id, bool isZone);
		~StarNode();
		
		/** @return node ID.
		*/
		int getID();
		/** @return true if node if a zone node, false otherwise
		*/
		bool getIsZone();
		/** @return internal node index.
		*/
		int getIndex();
		/** Sets internal node index.
		*/
		void setIndex(int index);
		
	private:
		const int id_;
		int index_;
		const bool isZone_;

};

#endif
