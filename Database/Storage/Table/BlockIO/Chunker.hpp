//
//  Chunker.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef Chunker_hpp
#define Chunker_hpp

#include "../Storable.hpp"

namespace ECE141 {
	class Chunker {
	public:
		Chunker(Storable* aStorable);

		Blocks chunk();
	protected:
		std::stringstream contents;
		Storable* storable;
		BlockHeader header;
	};
}

#endif // Chunker.hpp