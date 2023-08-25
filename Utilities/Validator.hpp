//
//  Table.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef Validator_hpp
#define Validator_hpp

#include "../Statements/DBQuery.hpp"

namespace ECE141 {
	struct Validator {
		// To check for uniqueness, we must pass in the entire table (schema + rows)
		static RowCollection insert(AttributeList aAttributeList, StringList& aIdentifierList, StringList& aValueList, StatusResult aResult) {
			RowCollection theRows;
			aResult = aValueList.size() % aIdentifierList.size() == 0 ? combine(aIdentifierList, aValueList, theRows) : Errors::insertSize;
			if (aIdentifierList.size() != aAttributeList.size()) {
				for (auto& theIdentifier : aIdentifierList) {
					auto theIter = std::find_if(aAttributeList.begin(), aAttributeList.end(), [&theIdentifier](const Attribute& theAttribute) {
						return theIdentifier == theAttribute.getName();
						});
					if(theIter != aAttributeList.end())
						aAttributeList.erase(theIter);
				}

				for (auto& theAttribute : aAttributeList) {
					aResult = theAttribute.isNull() ? Errors::noError : Errors::notNull;
				}
			}
			return theRows;
		}

		static StatusResult select(DBQuery* aQuery) {
			return Errors::noError;
		}

		// Helper Functions
		//-------------------------------------------------------------

		static StatusResult combine(StringList& aIdentifierList, StringList& aValueList, RowCollection& aRowList) {
			uint32_t theIdentCount = static_cast<uint32_t>(aIdentifierList.size());
			uint32_t theRowMax = (static_cast<uint32_t>(aValueList.size()) / theIdentCount) + 1;
			std::reverse(aValueList.begin(), aValueList.end());
			for (uint32_t theRowCount = 1; theRowCount < theRowMax; theRowCount++) {
				Row theRow(theRowCount);
				auto& theData = theRow.getData();
				for (uint32_t theCount = 0; theCount < theIdentCount; theCount++) {
					theData.insert(std::make_pair(aIdentifierList[theCount], aValueList.back()));
					aValueList.pop_back();
				}
				aRowList.push_back(std::make_unique<Row>(theRow));
			}
			return Errors::noError;
		}
	};
}

#endif // Validator.hpp