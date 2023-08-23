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
 /*   StatusResult  IndexStorable::encode(std::ostream& anOutput) {
        position = aBlockPos;
        //    nextIndex = position ? position : pointerIndex + 1;
        initHeader(BlockType::index_block, nextIndex);
        std::memset(payload, ' ', kPayloadSize);
        anOutput.read(payload, kPayloadSize);
        std::cout << payload;

        return Errors::noError;
    }
    bool          IndexStorable::save(std::fstream& aFile) {
        return write(aFile);
    }
    uint32_t& IndexStorable::getStorablePos() {
        return position;
    }
    StatusResult  IndexStorable::decode(std::istream& anInput) {
        return Errors::notImplemented;
    }
    */

    StatusResult  SchemaStorable::encode(std::ostream& anOutput) {
        std::stringstream thePayload;
        std::memset(payload, ' ', kPayloadSize);
        thePayload << header.type << " " << header.name;
        std::cout << thePayload.tellp();
        /*payload.write(contents, kBlockSize);
        while (numBlocks) {
            SchemaStorable* theSchema = new SchemaStorable();
            theSchema->setName(aSchema->getHash());
            theSchema->encode(pointerIndex, payload);
            blocks.push_back(std::make_unique<Storable*>(theSchema));
            pointerIndex++;
            numBlocks--;
        }
        std::memset(payload, ' ', kPayloadSize);
     //   anOutput.read(payload, kPayloadSize);
     */
        return Errors::noError;
    }

    bool SchemaStorable::save(std::fstream& aFile) {
        return write(aFile);
    }

    uint32_t& SchemaStorable::getStorablePos() {
        return position;
    }
    size_t& SchemaStorable::getName() { return getBlockName(); }
    StatusResult  SchemaStorable::decode(std::istream& anInput) { return Errors::notImplemented; }

    //StatusResult  DataStorable::encode(std::ostream& anOutput) const { return Errors::notImplemented; }
    //StatusResult  DataStorable::decode(std::istream& anInput) { return Errors::notImplemented; }
   // bool          DataStorable::initHeader(Block& aBlock) const { return false; }




//--------------------------------------------------------------
 // Storage 
  // USE: ctor ---------------------------------------

    Storage::Storage(const std::string& aName, AccessMode aMode)
        : BlockIO(aName, aMode) {}// auto load index first block with index pointing to all tables schemas only} and also count all blocks
    // USE: dtor ---------------------------------------
    Storage::~Storage() {
        if (changed)
            save();
    }//write all storables to storage 
  // create storables within add

    std::stringstream Storage::makePayload(StatementType aStmtType, DBQuery* aQuery) {
        std::stringstream thePayload;
        switch (aStmtType) {
        case StatementType::create:
            thePayload = schemaPayload(aQuery);
            break;
        case StatementType::insertTable:
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
    Storable* Storage::makeStorable(StatementType aStmtType, std::stringstream& aPayload, size_t aHash) {
        switch (aStmtType) {
        case StatementType::create:
            storable = new SchemaStorable(BlockType::schema_block, aPayload, pointerIndex, aHash);
            break;
        case StatementType::insertTable:
            break;
        default: break;
        }
        return storable;
    }

  StatusResult Storage::add(StatementType aStmtType, DBQuery* aQuery) {
      StatusResult theResult = Errors::noError;
      buffer = makePayload(aStmtType, aQuery);
      std::cout << buffer.str();
      while (!buffer.eof()) {
          if (auto* theStorable = makeStorable(aStmtType, buffer, aQuery->getSchema()->getHash())) { // 
              if (theStorable) {
                  changed = true;
                  theStorable->encode(stream);
              }
          }
      }

  return Errors::notImplemented; }

  // get all blocks with that table name, call block iterator
  StatusResult Storage::drop(StatementType aStmtType, DBQuery* aQuery) { return Errors::notImplemented; }
  bool Storage::each(const BlockVisitor &aVisitor) {
    return true;
  }

  StatusResult  Storage::indexBlock() {
  /*    std::stringstream payload;
      for (const auto& thePair : indices) {
          payload << *thePair.first << " " << thePair.second << "\\";
      }
      std::cout << payload.str();
      uint32_t numBlocks = chunk(payload.str());
      while (numBlocks) {
          IndexStorable* theIndex = new IndexStorable();
          if (numBlocks == 1)
              theIndex->encode(kFirstBlock, payload);
          else {
              theIndex->encode(pointerIndex, payload);
              pointerIndex++;
          }
          theIndex->save(stream);
      }*/
      return Errors::noError;
  }

  
  StatusResult Storage::dataBlock(const StringList& anIdentifierList, const StringList& aValueList) {
      //    SchemaStorable theSchema;
      return Errors::noError;
  }

  StatusResult Storage::schemaBlock(const Schema* aSchema) {
     /* std::stringstream payload(loadContents(aSchema->getAttributes()));
      Table theTable(aSchema);
      tables.push_back(theTable);

      uint32_t numBlocks = chunk(payload.str());
      changed = numBlocks ? true: false;
      while (numBlocks) {
          SchemaStorable* theSchema = new SchemaStorable();
          theSchema->setName(aSchema->getHash());
          theSchema->encode(pointerIndex, payload);
          blocks.push_back(std::make_unique<Storable*>(theSchema));
          pointerIndex++;
          numBlocks--;
      }
      //    SchemaStorable theSchema;*/
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

