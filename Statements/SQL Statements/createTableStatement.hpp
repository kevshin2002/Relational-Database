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
			StatusResult theResult = Errors::identifierExpected;
			if (aTokenizer.skipTo(TokenType::identifier)) {
				theResult = Errors::noError;
				tableName = aTokenizer.current().data;
				// parse table creation fields
			}
			return theResult;
		}
	};
}
#endif // createTableStatement.hpp
