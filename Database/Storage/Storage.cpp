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
    StatusResult  IndexStorable::encode() {
        std::stringstream thePayload;
        char theBuffer[kPayloadSize];
        std::memset(theBuffer, ' ', kPayloadSize);

        contents.read(theBuffer, kPayloadSize);
        thePayload << header.type << " " << nextIndex << " ";
        thePayload << theBuffer;

        thePayload.read(payload, kBlockSize);
        return Errors::noError;
    }
    bool          IndexStorable::save(std::fstream& aFile) {
        return write(aFile);
    }
    uint32_t& IndexStorable::getStorablePos() {
        return position;
    }
    uint32_t& IndexStorable::getName() { return getBlockName(); }

    StatusResult  IndexStorable::decode(std::istream& anInput) {
        return Errors::notImplemented;
    }
    


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

    bool SchemaStorable::save(std::fstream& aFile) {
        return write(aFile);
    }

    uint32_t& SchemaStorable::getStorablePos() {
        return position;
    }
    uint32_t& SchemaStorable::getName() { return getBlockName(); }
    StatusResult  SchemaStorable::decode(std::istream& anInput) { return Errors::notImplemented; }

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
    bool DataStorable::save(std::fstream& aFile) {
        return write(aFile);
    }

    uint32_t& DataStorable::getStorablePos() {
        return position;
    }
    uint32_t& DataStorable::getName() { return getBlockName(); }
    StatusResult  DataStorable::decode(std::istream& anInput) { return Errors::notImplemented; }




//--------------------------------------------------------------
 // Storage 
  // USE: ctor ---------------------------------------

    Storage::Storage(const std::string& aName, AccessMode aMode)
        : BlockIO(aName, aMode) {}// auto load index first block with index pointing to all tables schemas only} and also count all blocks
    // USE: dtor ---------------------------------------
    Storage::~Storage() {
        if (changed)
            save();
    }

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
                << "unique:" << theAttribute.isUnique() << "\\";
        }
        
        Table theTable(theSchema);
        getTables().push_back(theTable);
        return theContents;
    }

    std::stringstream Storage::dataPayload(DBQuery* aQuery) {
        std::stringstream theContents;
   //     auto& theIdentifierList = aQuery->getIdentifiers();
    //    auto& theValuesList = aQuery->getValues();
        return theContents;
    }
    Storable* Storage::makeStorable(StatementType aStmtType, std::stringstream& aPayload, uint32_t aHash) {
        switch (aStmtType) {
        case StatementType::create:
            storable = new SchemaStorable(BlockType::schema_block, aPayload, pointerIndex++, aHash);
            break;
        case StatementType::insertTable:
            storable = new DataStorable(BlockType::data_block, aPayload, pointerIndex++, aHash);
            break;
        default: break;
        }
        return storable;
    }

    Schema* Storage::getSchema(const std::string& aName) {
        return nullptr;
    }
    StatusResult Storage::add(StatementType aStmtType, DBQuery* aQuery) {
        StatusResult theResult = Errors::noError;
        buffer = makePayload(aStmtType, aQuery);
        while (!buffer.eof()) {
            if (auto* theStorable = makeStorable(aStmtType, buffer, aQuery->getSchema()->getHash())) { // 
                if (theStorable) {
                    changed = true;
                    theStorable->encode();
                    blocks.push_back(std::make_unique<Storable*>(theStorable));
                }
            }
        }
        return Errors::noError;
    }

  // get all blocks with that table name, call block iterator
  StatusResult Storage::drop(StatementType aStmtType, DBQuery* aQuery) { return Errors::notImplemented; }
  bool Storage::each(const BlockVisitor &aVisitor) {
    return true;
  }

  StatusResult  Storage::indexBlock() {
      std::stringstream payload;
      for (const auto& thePair : indices) {
          payload << *thePair.first << " " << thePair.second << "\\";
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

  bool Storage::save() {
      for (const auto& theBlock : blocks) {
          const auto& theContents = *theBlock.get();
          theContents->save(stream);
          indices.insert(std::make_pair(&(theContents->getStorablePos()), theContents->getName()));
      }
      indexBlock();
      return true;
  }
  std::string Storage::loadContents(const AttributeList& anAttributeList) {
      std::stringstream contents;
      for (const auto& theAttribute : anAttributeList) {
          contents << theAttribute.getName() << " ";

          if (theAttribute.getType() == DataTypes::varchar_type)
              contents << Helpers::dataTypeToString(theAttribute.getType()) << " " << theAttribute.getSize() << " ";
          else
              contents << Helpers::dataTypeToString(theAttribute.getType()) << " ";
          
          contents << "primary:" << theAttribute.isPrimary() << " "
                   << "increment:" << theAttribute.isIncrement() << " "
                   << "nullable:" << theAttribute.isNull() << " "
                   << "unique:" << theAttribute.isUnique() << "\\";
      }
      return contents.str();
  }
 



}

