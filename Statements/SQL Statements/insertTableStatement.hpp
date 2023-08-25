//
//  insertTableStatement.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef insertTableStatement_hpp
#define isnertTableStatement_hpp

#include "SQLStatement.hpp"

namespace ECE141 {
	class insertTableStatement : public SQLStatement {
	public:
		insertTableStatement(UniqueDatabase& aDatabase, StatementType theType) : SQLStatement(aDatabase, theType) {}
		StatusResult  parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::noError;
			ParseHelper theHelper(aTokenizer);
			TableName theName;

			theResult = aTokenizer.skipTo(TokenType::identifier) ? theHelper.parseTableName(theName) : Errors::identifierExpected;
			theResult = aTokenizer.skipIf(left_paren) ? theResult : Errors::insertIdentifiersOpenerExpected;
			if (theResult) {
				StringList theIdentifiers;
				query->getSchema()->setName(theName);
				theResult = theHelper.parseIdentifierList(theIdentifiers);
				if (aTokenizer.previous().data[0] == right_paren) {
					theResult = aTokenizer.skipIf(Keywords::values_kw) ? 
								aTokenizer.skipIf(left_paren) ? theResult : Errors::insertValuesOpenerExpected : 
								Errors::valueKeywordExpected;
					if (theResult) {
						StringList theValues;
						while (theResult && aTokenizer.more()) {
							theResult = theHelper.parseValueList(theValues);
							if(aTokenizer.more())
								theResult = aTokenizer.skipIf(comma) ?
										aTokenizer.skipIf(left_paren) ? theResult : Errors::syntaxError :
										Errors::syntaxError;
								
						}
						query->addIdentifiers(theIdentifiers);
						query->addValues(theValues);
					}
				}
				else
					theResult = Errors::insertIdentifiersCloserExpected;
			}

			if (theResult && aTokenizer.more())
				theResult = aTokenizer.skipIf(right_paren) ? theResult : Errors::insertValuesCloserExpected;
			else if(theResult)
				theResult = aTokenizer.previous().data[0] == right_paren ? theResult : Errors::insertValuesCloserExpected;

			return theResult;
		}
	};
}
#endif // insertTableStatement.hpp
