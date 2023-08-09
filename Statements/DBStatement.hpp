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

#include "../Controller/AppController.hpp"
#include "Statement.hpp"

namespace ECE141 {
	class DBStatement : public Statement {
	public:
		DBStatement(AppController* anAppController, StatementType aType) : appController(anAppController), Statement(aType) {}
		StatusResult  parse(Tokenizer& aTokenizer) override {
			aTokenizer.restart();
			StatementType theType = getType();
			StatusResult theResult = Errors::noError;

			switch (theType) {
			case StatementType::show:
				theResult = aTokenizer.skipTo(Keywords::databases_kw) ? Errors::noError : Errors::unknownCommand;
				break;
			case StatementType::unknown:
				theResult = Errors::unknownCommand;
				break;
			default:
				if (theType == StatementType::dumpDB) {
					theResult = aTokenizer.skipTo(Keywords::database_kw) ?
						aTokenizer.skipTo(TokenType::identifier) ? Errors::noError : Errors::identifierExpected :
						Errors::unknownCommand;
				}
				else
					theResult = aTokenizer.skipTo(TokenType::identifier) ? Errors::noError : Errors::identifierExpected;
				name = theResult ? aTokenizer.current().data : "";
				break;
			}
			aTokenizer.eof();
			return theResult;
		}

		const std::string& getDBName() const {return name;}
		AppController* getAppController() { return appController; }
		
	protected:
		std::string name;
		AppController* appController = nullptr;
	};
}
#endif //DBStatement.hpp