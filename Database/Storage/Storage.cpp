//
//  Storage.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//


#include "Storage.hpp"

namespace ECE141 {

// Storage 
//--------------------------------------------------------------
    // auto load index first block with index pointing to all tables schemas only} and also count all blocks
    Storage::Storage(const std::string& aPath, AccessMode aMode)
        : BlockIO(aPath, aMode), buffer(tables) {
        if (std::filesystem::file_size(aPath))
            initialize();
    }
    // USE: dtor ---------------------------------------
    Storage::~Storage() {
        if (changed)
            save();
     }

    StatusResult Storage::read(StatementType aStmtType, DBQuery* aQuery) {
        /*
        
        */
        return Errors::notImplemented;
    }

    StatusResult Storage::add(StatementType aStmtType, DBQuery* aQuery) {
        StatusResult theResult = Errors::noError;
        const auto& theSchema = aQuery->getSchema();
        const auto& theBuffer = buffer.getOutput();
        buffer.makePayload(aStmtType, aQuery);
        
        while (!theBuffer.eof()) {
            if (auto* theStorable = buffer.makeStorable(aStmtType, pointerIndex, theSchema->getName(), theSchema->getHash())) { // 
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
   

  StatusResult  Storage::indexBlock() {
      auto& theBuffer = buffer.getInput();
      for (const auto& thePair : indices) {
          theBuffer << thePair.second << " " << *thePair.first << " \\ ";
      }

      uint32_t numBlocks = chunk(theBuffer.str());
      for (uint32_t theCount = 1; theCount <= numBlocks; theCount++) {
          uint32_t newIndex = theCount == 1 ? kFirstBlock : pointerIndex++;
          uint32_t nextIndex = numBlocks == 1 ? kFirstBlock : pointerIndex;
          IndexBlock* theIndex = new IndexBlock(BlockType::index_block, theBuffer, newIndex, nextIndex, static_cast<uint32_t>(std::hash<std::string>{}(kTableIndex))); // feel like i should make this more readable...
          theIndex->encode();
          numBlocks--;  
          theIndex->save(stream);
      }
      return Errors::noError;
  }

  bool Storage::initialize() {
  uint32_t thePos = kFirstBlock;
  auto& theBuffer = buffer.getInput();
  Index:
      std::unique_ptr<IndexBlock> theIndex = std::make_unique<IndexBlock>(BlockType::index_block, theBuffer, thePos);
      load(*theIndex);
      theBuffer << theIndex->getPayload();
      if (!theIndex->getNextPos()) {
          thePos = theIndex->getNextPos();;
          goto Index;
      }
      return true;
  }
  bool Storage::save() {
      for (const auto& theBlock : blocks) {
          auto& theContents = *theBlock.get();
          theContents->save(stream);
          if (theContents->getType() == BlockType::schema_block)
              indices.insert(std::make_pair(&(theContents->getStorablePos()), theContents->getName()));
      }
      indexBlock();
      return true;
  }

  bool Storage::load(BlockIterator& anIterator) {
      return anIterator.retrieve(stream);
  }
 



}

