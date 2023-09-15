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
	Chunker::Chunker(Storable* aStorable) : storable(aStorable) {
        *aStorable >> contents;
       // std::cout << contents.str();
        header = storable->initHeader();
	}

	Blocks Chunker::chunk() {
        Blocks theBlocks;
        while (!contents.eof()) {
            Block theBlock;

            std::memset(theBlock.payload, ' ', kPayloadSize);
            theBlock.header = header;
            contents.read(theBlock.payload, kPayloadSize); // maybe fix 1013 and 1024
            theBlocks.push_back(theBlock);
        }
        return theBlocks;
	}
}