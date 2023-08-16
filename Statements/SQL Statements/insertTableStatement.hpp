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
		insertTableStatement(AppController* anAppController, StatementType theType) : SQLStatement(anAppController, theType) {}
		StatusResult  parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::noError;
			ParseHelper theHelper(aTokenizer);
			TableName theName;

			theResult = aTokenizer.skipTo(TokenType::identifier) ? theHelper.parseTableName(theName) : Errors::identifierExpected;
			theResult = aTokenizer.skipIf(left_paren) ? theResult : Errors::openerExpected;
			if (theResult) {
				StringList theIdentifiers;
				schema = theName.table;
				theResult = theHelper.parseIdentifierList(theIdentifiers);
				if (aTokenizer.previous().data[0] == right_paren) {
					theResult = aTokenizer.skipIf(Keywords::values_kw) ? 
								aTokenizer.skipIf(left_paren) ? theResult : Errors::openerExpected : 
								Errors::valueExpected;
					if (theResult) {
						StringList theValues;
						theResult = theHelper.parseValueList(theValues);
						query->addIdentifiers(theIdentifiers);
						query->addValues(theValues);
					}
				}
				else
					theResult = Errors::closerExpected;
			}

			if (theResult && aTokenizer.more())
				theResult = aTokenizer.skipIf(right_paren) ? theResult : Errors::closerExpected;
			else if(theResult)
				theResult = aTokenizer.previous().data[0] == right_paren ? theResult : Errors::closerExpected;

			return theResult;
		}
	};
}
#endif // insertTableStatement.hpp
