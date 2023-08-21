//
//  createTableStatement.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef createTableStatement_hpp
#define createTableStatement_hpp

#include "SQLStatement.hpp"

namespace ECE141 {
	class createTableStatement : public SQLStatement {
	public:
		createTableStatement(Database* aDatabase, StatementType theType) : SQLStatement(aDatabase, theType) {}
		StatusResult  parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::noError;
			ParseHelper theHelper(aTokenizer);
			TableName theName;

			theResult = aTokenizer.skipTo(TokenType::identifier) ? theHelper.parseTableName(theName) : Errors::identifierExpected;
			if (aTokenizer.skipIf(left_paren)) {
				//schema = theName.table;
				while (theResult && aTokenizer.more()) {
					Attribute theAttribute;
					theResult = theHelper.parseAttribute(theAttribute);
					query->addAttribute(theAttribute);
				}
			}
			else if(theResult)
				theResult = Errors::creationOpenerExpected;
			
			if (theResult && aTokenizer.more())
				theResult = aTokenizer.skipIf(right_paren) ? theResult : Errors::creationCloserExpected;
			else if(theResult)
				theResult = aTokenizer.previous().data[0] == right_paren ? theResult : Errors::creationCloserExpected;

			return theResult;
		}
	};
}
#endif // createTableStatement.hpp
