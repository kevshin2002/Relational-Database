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
		SQLStatement(UniqueDatabase& aDatabase, StatementType aType) : Statement(aType), database(aDatabase) { query = new DBQuery(aDatabase.get()); }
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
				else {
					theResult = aTokenizer.skipTo(TokenType::identifier) ? Errors::noError : Errors::identifierExpected;
				}
				break;
			}

			if (theResult)
				identifier = aTokenizer.current().data;
			return theResult;
		}
		bool isConditional(Tokenizer& aTokenizer) {
			if (aTokenizer.more()) {
				switch (aTokenizer.current().keyword) {
				case Keywords::where_kw:
					aTokenizer.next();
				case Keywords::order_kw:
				case Keywords::group_kw:
				case Keywords::limit_kw:
					return true;
				default:
					break;
				}
			}
			return false;
		}
		UniqueDatabase& getDatabase() { return database; }
		DBQuery* getQuery()  { return query; };
		std::string& getIdentifier() { return identifier; }
	protected:
		UniqueDatabase& database;
		std::string identifier;
		DBQuery* query; // consider making all pointers into smart pointers. reminder
	};
}

#endif // SQLStatement.hpp