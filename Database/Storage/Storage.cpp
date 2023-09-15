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
        : BlockIO(aPath, aMode) {
       // if (std::filesystem::file_size(aPath))
          //  initialize();
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
        changed = true;
        if (aStmtType == StatementType::create) {
            auto theSchema = aQuery->getSchema();
            Table theTable(theSchema);
            tables.push_back(theTable);
            storables.push_back(theSchema);
        }
        else
            storables.push_back(&aQuery->getRows());
        return Errors::noError;
    }

    StatusResult Storage::drop(StatementType aStmtType, DBQuery* aQuery) { return Errors::notImplemented; }

    bool Storage::each(const BlockVisitor& aVisitor) {
        
    }
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
    bool Storage::initialize() {
        /*
        * bool theResult = false;
        * theResult = index config ? indexIterator.each : each;
        * if(!theResult)
        *   each;
        if(index config){
            indexIterator.each(readBlock);
        
        */
        return true;
    }
    bool Storage::save() {
        for (const auto& theStorable : storables) {
            Chunker theChunker(theStorable);
            Blocks theBlocks = theChunker.chunk();
            uint32_t& thePosition = getFreeBlock(theBlocks.size());
            for (auto& theBlock : theBlocks) {
                writeBlock(thePosition++, theBlock);
            }
        }
        return true;
   }

   uint32_t& Storage::getFreeBlock(size_t aCount) {
        auto theIterator = freeBlocks.begin();

        while (theIterator != freeBlocks.end()) {
            auto first = *theIterator;

            if (std::all_of(theIterator, theIterator + aCount, [&first](uint32_t val) { return val == first++; })) {
                return *theIterator;
            }

            ++theIterator;
        }

        return pointerIndex;
    }




}

