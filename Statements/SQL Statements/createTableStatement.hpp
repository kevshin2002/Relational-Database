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
		createTableStatement(AppController* anAppController, StatementType theType) : SQLStatement(anAppController, theType) {}
		StatusResult  parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::noError;
			ParseHelper theHelper(aTokenizer);
			TableName theName;

			theResult = aTokenizer.skipTo(TokenType::identifier) ? theHelper.parseTableName(theName) : Errors::identifierExpected;
			theResult = aTokenizer.skipIf(left_paren) ? theResult : Errors::openerExpected;
			if (theResult) {
				schema = theName.table;
				while (theResult && aTokenizer.more()) {
					Attribute theAttribute;
					theResult = theHelper.parseAttribute(theAttribute);
					query->addAttribute(theAttribute);
				}
			}
			
			if (theResult && aTokenizer.more())
				theResult = aTokenizer.skipIf(right_paren) ? theResult : Errors::closerExpected;
			else if(theResult)
				theResult = aTokenizer.previous().data[0] == right_paren ? theResult : Errors::closerExpected;

			return theResult;
		}
	};
}
#endif // createTableStatement.hpp
