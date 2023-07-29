//
//  DBStatement.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef DBStatement_hpp
#define DBStatement_hpp


#include "Statement.hpp"

namespace ECE141 {
	class DBStatement : public Statement {
	public:
		DBStatement(AppProcessor* anAppController, StatementType aType) : appController(anAppController), Statement(aType) {}
		StatusResult	 virtual parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::noError;
			switch (getType()) {
			case StatementType::showDB:
				theResult = aTokenizer.skipTo(Keywords::databases_kw) ? Errors::noError : Errors::unknownCommand;
				break;
			case StatementType::unknown:
				theResult = Errors::unknownCommand;
				break;
			default:
				theResult = aTokenizer.skipTo(TokenType::identifier) ? Errors::noError : Errors::identifierExpected;
				break;
			}
			return theResult;
		}

	protected:
		std::string name;
		AppProcessor* appController;
	};
}
#endif //DBStatement.hpp