//
//  SQLStatement.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef SQLStatement_hpp
#define SQLStatement_hpp

#include "../Controller/AppController.hpp"
#include "Statement.hpp"


namespace ECE141 {
	class SQLStatement : public Statement {
	public:
		SQLStatement(AppController* anAppController, StatementType aType) : appController(anAppController), Statement(aType) {}
		StatusResult  parse(Tokenizer& aTokenizer) override { return Errors::notImplemented; }
	protected:
		AppController* appController = nullptr;
	};
}

#endif // SQLProcessor.hpp