//
//  Storage.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//


#include <sstream>
#include <cmath>
#include <cstdlib>
#include <optional>
#include <cstring>
#include <iostream>
#include "Storage.hpp"
#include "../../Utilities/Config.hpp"

namespace ECE141 {
// IndexStorable
//--------------------------------------------------------------
    StatusResult  IndexStorable::encode() {
        std::stringstream thePayload;
        char theBuffer[kPayloadSize + typeError];
        std::memset(theBuffer, ' ', kPayloadSize + typeError);

        contents.read(theBuffer, kPayloadSize + typeError);
        thePayload << header.type << " " << nextIndex << " ";
        thePayload << theBuffer;

        thePayload.read(payload, kBlockSize);
        return Errors::noError;
    }
    StatusResult  IndexStorable::decode(Storage& aStorage) {
        auto& theIndices = aStorage.getIndices();
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

    bool          IndexStorable::save(std::fstream& aFile) {
        return write(aFile);
    }
    uint32_t&     IndexStorable::getStorablePos() {
        return position;
    }
    BlockType     IndexStorable::getType() { return static_cast<BlockType>(header.type); }
    uint32_t&     IndexStorable::getHash() { return getHashName(); }
    std::string&  IndexStorable::getName() { return getIdentifierName(); }
 

 // SchemaStorable
 //--------------------------------------------------------------

    StatusResult  SchemaStorable::encode() {
        std::stringstream thePayload;
        char theBuffer[kPayloadSize];
        std::memset(theBuffer, ' ', kPayloadSize);

        contents.read(theBuffer, kPayloadSize); 
        thePayload << header.type << " " << header.name << " ";
        thePayload << theBuffer;

        thePayload.read(payload, kBlockSize);
        return Errors::noError;
    }

    StatusResult  SchemaStorable::decode(Storage& aStorage) { return Errors::notImplemented; }
    bool          SchemaStorable::save(std::fstream& aFile) {
        return write(aFile);
    }

    uint32_t& SchemaStorable::getStorablePos() {
        return position;
    }
    BlockType     SchemaStorable::getType() { return static_cast<BlockType>(header.type); }
    uint32_t&     SchemaStorable::getHash() { return getHashName(); }
    std::string&  SchemaStorable::getName() { return getIdentifierName(); }


 // DataStorable
 //--------------------------------------------------------------

    StatusResult  DataStorable::encode() {
        std::stringstream thePayload;
        char theBuffer[kPayloadSize];
        std::memset(theBuffer, ' ', kPayloadSize);

        contents.read(theBuffer, kPayloadSize);
        thePayload << header.type << " " << header.name << " ";
        thePayload << theBuffer;

        thePayload.read(payload, kBlockSize);
        return Errors::noError;
    }

    StatusResult  DataStorable::decode(Storage& aStorage) { return Errors::notImplemented; }
    bool          DataStorable::save(std::fstream& aFile) {
        return write(aFile);
    }

    uint32_t&     DataStorable::getStorablePos() {
        return position;
    }
    BlockType     DataStorable::getType() { return static_cast<BlockType>(header.type); }
    uint32_t&     DataStorable::getHash() { return getHashName(); }
    std::string&  DataStorable::getName() { return getIdentifierName(); }


// Storage 
//--------------------------------------------------------------

    Storage::Storage(const std::string& aPath, AccessMode aMode)
    : BlockIO(aPath, aMode){
        load_meta();
    }// auto load index first block with index pointing to all tables schemas only} and also count all blocks
    // USE: dtor ---------------------------------------
    Storage::~Storage() {
        if (changed)
            save();
     }

    StatusResult Storage::read(StatementType aStmtType, DBQuery* aQuery) {
        return Errors::notImplemented;
    }

    StatusResult Storage::add(StatementType aStmtType, DBQuery* aQuery) {
        StatusResult theResult = Errors::noError;
        const auto& theSchema = aQuery->getSchema();
        buffer = makePayload(aStmtType, aQuery);
        while (!buffer.eof()) {
            if (auto* theStorable = makeStorable(aStmtType, buffer, theSchema->getName(), theSchema->getHash())) { // 
                if (theStorable) {
                    changed = true;
                    theStorable->encode();
                    blocks.push_back(std::make_unique<Storable*>(theStorable));
                }
            }
        }
        return Errors::noError;
    }

    StatusResult Storage::drop(StatementType aStmtType, DBQuery* aQuery) { return Errors::notImplemented; }

    StatusResult Storage::fetchTables(std::set<std::string>& aTableList) {
        for (auto& theTable : tables) {
            aTableList.insert(theTable.getSchema().getName());
        }
        return Errors::noError;
    }
    Schema* Storage::getSchema(const std::string& aName) {
        if (auto* theTable = getTable(aName)) {
            return &theTable->getSchema();
        }
        return nullptr;
    }

    Table* Storage::getTable(const std::string& aName) {
        for (auto& theTable : tables) {
            if (theTable.getSchema().getName() == aName)
                return &theTable;
        }
        return nullptr;
    }


    // Non-exposed API
    //--------------------------------------------------------------
    std::stringstream Storage::makePayload(StatementType aStmtType, DBQuery* aQuery) {
        std::stringstream thePayload;
        switch (aStmtType) {
        case StatementType::create:
            thePayload = schemaPayload(aQuery);
            break;
        case StatementType::insertTable:
            thePayload = dataPayload(aQuery);
            break;
        default:
            break;
        }
        return thePayload;
    }

    Storable* Storage::makeStorable(StatementType aStmtType, std::stringstream& aPayload, std::string aName, uint32_t aHash) {
        switch (aStmtType) {
        case StatementType::create:
            storable = new SchemaStorable(BlockType::schema_block, aPayload, pointerIndex++, aName, aHash);
            break;
        case StatementType::insertTable:
            storable = new DataStorable(BlockType::data_block, aPayload, pointerIndex++, aName, aHash);
            break;
        default: break;
        }
        return storable;
    }

    std::stringstream Storage::schemaPayload(DBQuery* aQuery) {
        std::stringstream theContents;
        auto* theSchema = aQuery->getSchema();
        auto& theAttributeList = theSchema->getAttributes();
        for (const auto& theAttribute : theAttributeList) {
            theContents << theAttribute.getName() << " ";

            if (theAttribute.getType() == DataTypes::varchar_type)
                theContents << Helpers::dataTypeToString(theAttribute.getType()) << " " << theAttribute.getSize() << " ";
            else
                theContents << Helpers::dataTypeToString(theAttribute.getType()) << " ";

            theContents << "primary:" << theAttribute.isPrimary() << " "
                << "increment:" << theAttribute.isIncrement() << " "
                << "nullable:" << theAttribute.isNull() << " "
                << "unique:" << theAttribute.isUnique() << " \\ ";
        }
        
        Table theTable(theSchema);
        getTables().push_back(theTable);
        return theContents;
    }

    std::stringstream Storage::dataPayload(DBQuery* aQuery) {
        std::stringstream theContents;
        RowCollection& theRows = aQuery->getRows();
        for (auto& theRow : theRows) {
            auto& theDatas = theRow.get()->getData();
            for (auto& theData : theDatas) {
                theContents << theData.first << ":" << std::get<std::string>(theData.second) << " \\ ";
            }
        }
        return theContents;
    }

  StatusResult  Storage::indexBlock() {
      std::stringstream payload;
      for (const auto& thePair : indices) {
          payload << thePair.second << " " << *thePair.first << " \\ ";
      }

      uint32_t numBlocks = chunk(payload.str());
      for (uint32_t theCount = 1; theCount <= numBlocks; theCount++) {
          uint32_t newIndex = theCount == 1 ? kFirstBlock : pointerIndex++;
          uint32_t nextIndex = numBlocks == 1 ? kFirstBlock : pointerIndex;
          IndexStorable* theIndex = new IndexStorable(BlockType::index_block, payload, newIndex, nextIndex, static_cast<uint32_t>(std::hash<std::string>{}(kTableIndex))); // feel like i should make this more readable...
          theIndex->encode();
          numBlocks--;  
          theIndex->save(stream);
      }
      return Errors::noError;
  }

  bool Storage::load_meta() {
      uint32_t position = kFirstBlock;
      uint32_t next = 0;
      do {
          std::stringstream theContents;
          load_Payload(position, theContents);
          IndexStorable* theStorable = new IndexStorable(BlockType::index_block, theContents, position, next, static_cast<uint32_t>(std::hash<std::string>{}(kTableIndex)));
          

      } while (next);
      Block theBlock;
      
      return true;
  }
  bool Storage::save() {
      for (const auto& theBlock : blocks) {
          const auto& theContents = *theBlock.get();
          theContents->save(stream);
          if (theContents->getType() == BlockType::schema_block)
              indices.insert(std::make_pair(&(theContents->getStorablePos()), theContents->getName()));
      }
      indexBlock();
      return true;
  }
 
  bool Storage::retrieve(const BlockVisitor& aVisitor) {
      return true;
  }


}

