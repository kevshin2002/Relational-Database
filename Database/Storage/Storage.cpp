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
  //  StatusResult  IndexStorable::encode(std::ostream& anOutput) const { return Errors::notImplemented; }
   // StatusResult  IndexStorable::decode(std::istream& anInput) { return Errors::notImplemented; }
   // bool          IndexStorable::initHeader(Block& aBlock) const { return false; }

    StatusResult  SchemaStorable::encode(std::stringstream& anOutput) {
        initHeader(BlockType::schema_block, hashedName);
        std::memset(payload, ' ', kPayloadSize);
        anOutput.read(payload, kPayloadSize);
        return Errors::noError;
    }

    bool SchemaStorable::save(std::fstream& aFile) {
        return write(aFile);
    }
    StatusResult  SchemaStorable::decode(std::istream& anInput) { return Errors::notImplemented; }

    //StatusResult  DataStorable::encode(std::ostream& anOutput) const { return Errors::notImplemented; }
    //StatusResult  DataStorable::decode(std::istream& anInput) { return Errors::notImplemented; }
   // bool          DataStorable::initHeader(Block& aBlock) const { return false; }

    


//--------------------------------------------------------------
 // Storage 
  // USE: ctor ---------------------------------------

    Storage::Storage(const std::string& aName, AccessMode aMode)
        : BlockIO(aName, aMode) {}// auto load index first block with index pointing to all tables schemas only}
  // USE: dtor ---------------------------------------
      Storage::~Storage() {
          for (const auto& theBlock : blocks) {
              (*theBlock.get())->save(stream);
          }
      }//write all storables to storage 
  // create storables within add
  StatusResult Storage::add(BlockType aType, StatementType aStmtType, DBQuery* aQuery) {
      StatusResult theResult = Errors::noError;
      switch (aStmtType) {
      case StatementType::create:
          theResult = schemaBlock(aQuery->getSchema());
          break;
      case StatementType::insertTable:
          theResult = dataBlock(aQuery->getIdentifiers(), aQuery->getValues());
          break;
      default:
          break;
      }
      return theResult;}
  // get all blocks with that table name, call block iterator
  StatusResult Storage::drop(BlockType aType, StatementType aStmtType, DBQuery* aQuery) { return Errors::notImplemented; }
  bool Storage::each(const BlockVisitor &aVisitor) {
    return true;
  }

  StatusResult Storage::schemaBlock(const Schema* aSchema) {
      std::stringstream payload(loadContents(aSchema->getAttributes()));
      Table theTable(aSchema);
      tables.push_back(theTable);

      uint32_t numBlocks = chunk(payload.str());
      for (uint32_t blocksCreated = 0; blocksCreated < numBlocks; blocksCreated++) {
          SchemaStorable* theSchema = new SchemaStorable();
          theSchema->setName(aSchema->getHash());
          theSchema->encode(payload);
          blocks.push_back(std::make_unique<Storable*>(theSchema));
      }
  //    SchemaStorable theSchema;
      return Errors::noError;
  }

  StatusResult Storage::dataBlock(const StringList& anIdentifierList, const StringList& aValueList) {
      //    SchemaStorable theSchema;
      return Errors::noError;
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

