//
//  selectTableStatement.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef selectTableStatement_hpp
#define selectTableStatement_hpp

#include "SQLStatement.hpp"

namespace ECE141 {
	class selectTableStatement : public SQLStatement {
	public:
		selectTableStatement(AppController* anAppController, StatementType theType) : SQLStatement(anAppController, theType) {}
		StatusResult  parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::invalidCommand;
			// query selector parsing

			if (aTokenizer.skipTo(Keywords::from_kw)) {
				theResult = Errors::identifierExpected;
				tableName = aTokenizer.skipTo(TokenType::identifier) ? aTokenizer.current().data : "none";
			}
			

			// condition parsing
			return theResult;
		}
	};
}
#endif // selectTableStatement.hpp
