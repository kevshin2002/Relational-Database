//
//  BufferManager.cpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#include "BufferManager.hpp"

namespace ECE141 {
  StatusResult BufferManager::makePayload(StatementType aStmtType, DBQuery* aQuery) {
      StatusResult theResult = Errors::noError; // make this a data related error
        switch (aStmtType) {
        case StatementType::create:
            outputBuffer = schemaPayload(aQuery);
            break;
        case StatementType::insertTable:
            outputBuffer = dataPayload(aQuery);
            break;
        default:
            break;
        }
        return theResult;
    }

    Storable* BufferManager::makeStorable(StatementType aStmtType, uint32_t& aPosition, std::string aName, uint32_t aHash) {
        switch (aStmtType) {
        case StatementType::create:
            storable = new SchemaBlock(BlockType::schema_block, outputBuffer, aPosition++, aName, aHash);
            break;
        case StatementType::insertTable:
            storable = new DataBlock(BlockType::data_block, outputBuffer, aPosition++, aName, aHash);
            break;
        default: break;
        }
        return storable;
    }

    std::stringstream BufferManager::schemaPayload(DBQuery* aQuery) {
        std::stringstream theContents;
        auto* theSchema = aQuery->getSchema();
        auto& theAttributeList = theSchema->getAttributes();
        Table theTable(theSchema);
        tables.push_back(theTable);

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
        return theContents;
    }

    std::stringstream BufferManager::dataPayload(DBQuery* aQuery) {
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
}