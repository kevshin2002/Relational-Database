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
		static StatusResult insert(AttributeList& aAttributeList, StringList& aIdentifierList, StringList& aValueList) {
			StatusResult theResult = Errors::noError;
			std::reverse(aValueList.begin(), aValueList.end());
			for (const auto& theIdentifier : aIdentifierList) {
				auto theIter = std::find_if(aAttributeList.begin(), aAttributeList.end(), [&theIdentifier](const Attribute& theAttribute) {
					return theIdentifier == theAttribute.getName();
					});
				auto theValue = aValueList.back();
				if (theIter != aAttributeList.end()) {
					/*
					uniqueness only
					auto increment = adds one
					primary key = 
					*/
				}
				else {
					theResult = theIter->isNull() ? theResult : Errors::notNull;
				}
				aValueList.pop_back();
			}
			return Errors::noError;
			/*
			
			
			
			*/
		}

		static StatusResult select(DBQuery* aQuery) {
			return Errors::noError;
		}
	};
}

#endif // Validator.hpp