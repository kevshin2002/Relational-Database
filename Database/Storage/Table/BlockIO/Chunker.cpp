//
//  Chunker.cpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#include "Chunker.hpp"

namespace ECE141 {
	Chunker::Chunker(Storable* aStorable) : storable(aStorable), name(storable->getName()) {
        *aStorable >> contents;
       // std::cout << contents.str();
        header = storable->initHeader();
	}

	Blocks Chunker::chunk() {
        Blocks theBlocks;
        uint32_t theSize = storable->getSize();
        while (!contents.eof()) {
            Block theBlock;
            theBlock.setName(name);
            std::memset(theBlock.payload, ' ', kPayloadSize);
            theBlock.header = header;
            contents.read(theBlock.payload, theSize); // maybe fix 1013 and 1024
            theBlocks.push_back(theBlock);
        }
        return theBlocks;
	}
}