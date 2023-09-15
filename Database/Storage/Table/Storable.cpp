
//
//  Storable.cpp
// 
//
//  Created by rick gessner on 2/27/23.
//

#include "Storable.hpp"

namespace ECE141 {
 /*   // IndexStorable
//--------------------------------------------------------------
    StatusResult  IndexBlock::encode() {
        std::stringstream thePayload;
        char theBuffer[kPayloadSize + typeError];
        std::memset(theBuffer, ' ', kPayloadSize + typeError);

        contents.read(theBuffer, kPayloadSize + typeError);
        thePayload << header.type << " " << nextIndex << " ";
        thePayload << theBuffer;

        thePayload.read(payload, kBlockSize);
        return Errors::noError;
    }
    StatusResult  IndexBlock::decode() {
        //    auto& theIndices = aStorage.getIndices();
        char theType;
        uint32_t nextPos = 0;

        contents >> theType;
        if (theType == static_cast<char>(BlockType::index_block)) {
            contents >> nextPos;
            nextIndex = nextPos;
            //     while(!contents.eof())
              //   contents >> theTableName;

        }
        nextIndex = nextPos;

        return Errors::notImplemented;
    }
    bool          IndexBlock::save(std::fstream& aFile) { return write(aFile); }
    bool          IndexBlock::retrieve(std::fstream& aFile) {
        read(aFile);
        contents << payload;
        return true;
    }

    uint32_t&     IndexBlock::getStorablePos() { return position; }
    BlockType     IndexBlock::getType() { return static_cast<BlockType>(header.type); }
    uint32_t&     IndexBlock::getHash() { return getHashName(); }
    std::string&  IndexBlock::getName() { return getIdentifierName(); }


    // SchemaStorable
    //--------------------------------------------------------------

    StatusResult  SchemaBlock::encode() {
        std::stringstream thePayload;
        char theBuffer[kPayloadSize];
        std::memset(theBuffer, ' ', kPayloadSize);

        contents.read(theBuffer, kPayloadSize);
        thePayload << header.type << " " << header.name << " ";
        thePayload << theBuffer;

        thePayload.read(payload, kBlockSize);
        return Errors::noError;
    }

    StatusResult  SchemaBlock::decode() { return Errors::notImplemented; }
    
    bool          SchemaBlock::save(std::fstream& aFile) { return write(aFile); }
    uint32_t&     SchemaBlock::getStorablePos() { return position; }
    BlockType     SchemaBlock::getType() { return static_cast<BlockType>(header.type); }
    uint32_t&     SchemaBlock::getHash() { return getHashName(); }
    std::string&  SchemaBlock::getName() { return getIdentifierName(); }


    // DataStorable
    //--------------------------------------------------------------

    StatusResult  DataBlock::encode() {
        std::stringstream thePayload;
        char theBuffer[kPayloadSize];
        std::memset(theBuffer, ' ', kPayloadSize);

        contents.read(theBuffer, kPayloadSize);
        thePayload << header.type << " " << header.name << " ";
        thePayload << theBuffer;

        thePayload.read(payload, kBlockSize);
        return Errors::noError;
    }

    StatusResult  DataBlock::decode() { return Errors::notImplemented; }
    bool          DataBlock::save(std::fstream& aFile) { return write(aFile); }

    uint32_t&     DataBlock::getStorablePos() { return position; }
    BlockType     DataBlock::getType() { return static_cast<BlockType>(header.type); }
    uint32_t&     DataBlock::getHash() { return getHashName(); }
    std::string&  DataBlock::getName() { return getIdentifierName(); }
    */
}