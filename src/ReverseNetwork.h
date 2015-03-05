class StarLink;
class StarNetwork;

/** \brief Factory class that creates a reversed network from a given network.

	The reversed network has all links reversed, i.e. they point in the opposite direction.
	\warning Internal link and node indexes in reversed network do not correspond to 
	internal indexes of original network.

	\author Benoit Mulocher
*/
class ReverseNetwork {

	public:
		ReverseNetwork();
		virtual ~ReverseNetwork();

		/** Creates a Reverse Star from a given Forward Star.
		*/
		StarNetwork* createReverseStar (const StarNetwork& net);
		/** Helper methods that checks if node indexes in both networks are the same.
			@return true if all indexes are the same, false otherwise.
		*/
		bool checkNodeIndexes(const StarNetwork& net, StarNetwork* reversedNet);

	protected:
		/** @return allocated link.
		*/
		virtual StarLink* createLink(StarLink* link);
	
};