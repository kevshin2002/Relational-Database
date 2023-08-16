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

#include "../../Controller/AppController.hpp"
#include "../Statement.hpp"
#include "../DBQuery.hpp"

namespace ECE141 {
	// good track
	// blockvisitor
	// dont rely on order of anything
	// make them independent
	// consider block 0 trick (state)
	// add it later (can add in indexing)
	// come up with a way to distinguish between different iterators
	// schemas 
	// hash schema id
	// BlockIterator
	// dont worry about edge cases


	class SQLStatement : public Statement {
	public:
		SQLStatement(AppController* anAppController, StatementType aType) : appController(anAppController), schema("none"), Statement(aType) {}
		StatusResult  parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::noError;
			StatementType theType = getType();
			aTokenizer.restart();

			switch (theType) {
			case StatementType::show:
				theResult = aTokenizer.skipTo(Keywords::tables_kw) ? theResult : Errors::invalidCommand;
				break;
			case StatementType::unknown:
				theResult = Errors::unknownCommand;
				break;
			default:
				if (theType == StatementType::drop) {
					theResult = aTokenizer.skipTo(Keywords::table_kw) ?
						aTokenizer.skipTo(TokenType::identifier) ? theResult : Errors::identifierExpected :
						Errors::invalidCommand;
				}
				else{
					theResult = aTokenizer.skipTo(TokenType::identifier) ? Errors::noError : Errors::identifierExpected;
				    schema = theResult ? aTokenizer.current().data : "none";
				}
				break;
			}
			return theResult;
		}

		AppController* getAppController() const { return appController; }
		const DBQuery* getQuery() const { return query; };
		
	protected:
		AppController* appController = nullptr;
		Schema schema;
		DBQuery* query = new DBQuery(); // consider making all pointers into smart pointers. reminder
	};
}

#endif // SQLProcessor.hpp