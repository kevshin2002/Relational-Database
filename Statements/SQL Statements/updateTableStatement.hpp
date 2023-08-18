//
//  updateTableStatement.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef updateTableStatement_hpp
#define updateTableStatement_hpp

#include "SQLStatement.hpp"

namespace ECE141 {
	class updateTableStatement : public SQLStatement {
	public:
		updateTableStatement(AppController* anAppController, StatementType theType) : SQLStatement(anAppController, theType) {}
		StatusResult parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::noError;

			aTokenizer.next();
			theResult = aTokenizer.skipIf(TokenType::identifier) ? query->getFilters().parse(aTokenizer, schema) : Errors::identifierExpected;
			return theResult;
		}
	};
}

#endif/