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
        if (std::filesystem::file_size(aPath))
            initialize();
    }
    Storage::Storage(const Storage& aStorage) : BlockIO(aStorage) {
        freeBlocks = aStorage.freeBlocks;
        storables = aStorage.storables;
        schemas = aStorage.schemas;
        rows = aStorage.rows;
    }
    // USE: dtor ---------------------------------------
    Storage::~Storage() {
        if (changed) {
            save();
            index(false);
        }
     }

    StatusResult Storage::read(StatementType aStmtType, DBQuery* aQuery) {
        changed = false;
        auto theName = aQuery->getSchema()->getName();
        StatusResult theResult =getData(theName);
        /*
        
        */
        return theResult;
    }

    StatusResult Storage::add(StatementType aStmtType, DBQuery* aQuery) {
        changed = true;
        if (aStmtType == StatementType::create) {
            auto theSchema = aQuery->getSchema();
            schemas.insert(std::make_pair(theSchema->getName(), *theSchema));
            storables.push_back(theSchema);
        }
        else {
            storables.push_back(&aQuery->getRows());
            rows = aQuery->getRows();
        }
        return Errors::noError;
    }

    StatusResult Storage::drop(const std::string& aName, DBQuery* aQuery) {
        changed = true;
        schemas.erase(aName);
        if (rows.getName() == std::to_string(Helpers::hashString(aName)))
            rows.clear();

        for (auto theIndex = indices.begin(); theIndex != indices.end(); theIndex++) {
            if (theIndex->first.second == aName || theIndex->first.second == std::to_string(Helpers::hashString(aName))) {
                theIndex->second = static_cast<char>(BlockType::free_block);
            }
        }

        storables.erase(std::remove_if(storables.begin(), storables.end(), [&aName](Storable* aStorable) {
                return aStorable->getName() == aName;
                }),
            storables.end()
        );
        return Errors::noError;
    }

    bool Storage::each(const BlockVisitor& aVisitor) {
        Block theBlock;
        bool theResult = false;
        while (!theResult) {
            theResult = aVisitor(pointerIndex, theBlock);
            pointerIndex++;
        }
        return theResult;

    }
#include <cctype> // Include the header for std::isdigit

    StatusResult Storage::getData(std::string aName) {
        std::vector<Block> theBlockList;
        pointerIndex = kFirstBlock;

        each([&theBlockList, this](uint32_t& aBlockNum, Block& aBlock) {
            readBlock(aBlockNum, aBlock);
            if (aBlock.header.type == static_cast<char>(BlockType::glossary_block)) {
                auto theKey = std::make_pair(aBlockNum, aBlock.name);
                indices.insert(std::make_pair(theKey, aBlock.header.type));
                theBlockList.push_back(aBlock);
            }
            return aBlock.header.offset == 0 ? true : false;
            });

        std::stringstream theStream;
        std::for_each(theBlockList.begin(), theBlockList.end(), [&theStream](const Block& theBlock) {
            theStream << theBlock.payload;
            });

        rows << theStream;
        return Errors::noError;
    }

    StatusResult Storage::fetchTables(std::set<std::string>& aTableList) {
        // Extract unique strings using std::transform and std::set constructor
        std::transform(indices.begin(), indices.end(),
            std::inserter(aTableList, aTableList.end()),
            [](const auto& pair) {
                const std::string& tableName = pair.first.second;

                return std::all_of(tableName.begin(), tableName.end(), [](unsigned char c) { return std::isdigit(c); }) ?
                    std::string() :
                    tableName;
            });
        aTableList.erase("");

        return Errors::noError;
    }

    Schema* Storage::getSchema(const std::string& aName) {
        bool inSchema = schemas.count(aName);
        if (!inSchema) {
            auto theID = std::find_if(indices.begin(), indices.end(), [&aName](const auto& theKey) {
                return theKey.first.second == aName;
                });
            if (theID != indices.end()) {
                Block theBlock(static_cast<BlockType>(theID->second), theID->first.first, theID->first.second, Helpers::hashString(theID->first.second));
                readBlock(theID->first.first, theBlock);
                Schema theSchema(theID->first.second);
                std::stringstream theStream(theBlock.payload);
                theSchema << theStream;
                schemas.insert(std::make_pair(theID->first.second, theSchema));
                inSchema = true;
            }
        }
        return inSchema ? &schemas.find(aName)->second : nullptr;
    }
    // Can I get rid of the &*?
    // Non-exposed API
    //--------------------------------------------------------------
    bool Storage::initialize() {
        std::vector<Block> theBlockList;
        pointerIndex = kFirstBlock;

        // Fetch all the glossary blocks
        each([&theBlockList, this](uint32_t& aBlockNum, Block& aBlock) {
                readBlock(aBlockNum, aBlock);
                if (aBlock.header.type == static_cast<char>(BlockType::glossary_block)) {
                    auto theKey = std::make_pair(aBlockNum, aBlock.name);
                    indices.insert(std::make_pair(theKey, aBlock.header.type));
                    theBlockList.push_back(aBlock);
                }
                return aBlock.header.offset == 0 ? true : false;
            });

        // Process the glossary blocks contents into different blocks.
        std::stringstream theStream;
        std::for_each(theBlockList.begin(), theBlockList.end(), [&theStream](const Block& theBlock) {
            theStream << theBlock.payload;
            });
        
        while (theStream) {
            std::string theName;
            char theType = 'U';
            uint32_t thePosition = 0;
            theStream >> theName
                >> theType
                >> thePosition;
            theStream.skipws;
            theStream.ignore(2);

            // Consider putting this into a function to reduce redundancy.
            if (isBlockType(static_cast<BlockType>(theType)) ){
            auto theKey = std::make_pair(thePosition, theName);
            indices.insert(std::make_pair(theKey, theType));
            }
        }
        return true;
    }
    bool Storage::save() {
        for (const auto& theStorable : storables) {
            Chunker theChunker(theStorable);
            Blocks theBlocks = theChunker.chunk();
            size_t theSize = theBlocks.size();
            uint32_t& thePosition = getFreeBlock(theSize);
            for (auto& theBlock : theBlocks) {
                auto theKey = std::make_pair(thePosition, theBlock.name);
                indices.insert(std::make_pair(theKey, theBlock.header.type));
                if (theSize > -kNewBlock) {
                    theBlock.setOffset(Offset_One);
                    theSize--;
                }
                writeBlock(thePosition++, theBlock);
                
            }
        }

        for (auto theIndex = indices.begin(); theIndex != indices.end();) {
            if (theIndex->second == static_cast<char>(BlockType::free_block)) {
                Block theBlock;
                theBlock.header.type = 'F';
                writeBlock(theIndex->first.first, theBlock);
                theIndex = indices.erase(theIndex);
            }
            else
                theIndex++;
        }
        return true;
   }
    bool Storage::index(bool aConfig) {
        Index theIndex(indices);
        Chunker theChunker(&theIndex);
        Blocks theBlocks = theChunker.chunk();
        size_t theSize = theBlocks.size();
        uint32_t& thePosition = getFreeBlock(theSize);

        // I don't like this, find a way to shorten.
        if (indices.find(std::make_pair(kFirstBlock, kTableIndex)) != indices.end()) {
            for (auto& theBlock : theBlocks) {
                if (theSize > -kNewBlock) {
                    theBlock.setOffset(Offset_One);
                    theSize--;
                }
                writeBlock(thePosition++, theBlock);
            }
        }
        else {
            auto theIterator = theBlocks.begin();
            if (theSize > -kNewBlock) {
                theIterator->setOffset(thePosition);
                theSize--;
            }
            writeBlock(kFirstBlock, *theIterator++);
            while (theIterator != theBlocks.end()) {
                if (theSize > -kNewBlock) {
                    theIterator->setOffset(Offset_One);
                    theSize--;
                }
                writeBlock(thePosition++, *theIterator++);
            };
            

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
        return getBlockCount();
    }




}

