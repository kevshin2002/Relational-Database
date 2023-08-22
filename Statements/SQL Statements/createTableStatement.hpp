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

			theResult = aTokenizer.skipTo(TokenType::identifier) ? theHelper.parseTableName(theName) : StatusResult(Errors::identifierExpected, aTokenizer.pos());
			if (aTokenizer.skipIf(left_paren)) {
				query->getSchema()->setName(theName);
				while (theResult && aTokenizer.more()) {
					Attribute theAttribute;
					theResult = theHelper.parseAttribute(theAttribute);
					query->getSchema()->addAttribute(theAttribute);
				}
			}
			else if(theResult)
				theResult = StatusResult(Errors::creationOpenerExpected, aTokenizer.pos());
			
			if (theResult && aTokenizer.more())
				theResult = aTokenizer.skipIf(right_paren) ? theResult : StatusResult(Errors::creationCloserExpected, aTokenizer.pos());
			else if(theResult)
				theResult = aTokenizer.previous().data[0] == right_paren ? theResult : StatusResult(Errors::creationCloserExpected, aTokenizer.prevPos());

			return theResult;
		}
	};
}
#endif // createTableStatement.hpp
