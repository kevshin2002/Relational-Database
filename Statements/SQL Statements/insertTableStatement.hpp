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
			StatusResult theResult = Errors::identifierExpected;
			// Check for semantics: insert into table name values
			if (aTokenizer.skipTo(Keywords::into_kw) && aTokenizer.skipTo(TokenType::identifier)) {
				theResult = Errors::noError;
				tableName = aTokenizer.current().data;
			}

			// parse values
			return theResult;
		}
	};
}
#endif // insertTableStatement.hpp
