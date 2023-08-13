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
#include "../../Utilities/ParseHelper.hpp"
#include "../Statement.hpp"

namespace ECE141 {

	/* is this the right approach? */
	struct SQLQuery {
		const AttributeList& getAttributes() const { return attributes; }
		SQLQuery& addAttribute(Attribute& anAttribute) {
			attributes.push_back(anAttribute);
			return *this;
		}

		AttributeList attributes;
	};
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
		SQLStatement(AppController* anAppController, StatementType aType) : appController(anAppController), tableName("none"), Statement(aType) {}
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
				    tableName = theResult ? aTokenizer.current().data : "none";
				}
				break;
			}
			return theResult;
		}

		AppController* getAppController() const { return appController; }
		virtual SQLQuery* getQuery() const { return query; };
		
	protected:
		AppController* appController = nullptr;
		TableName tableName;
		SQLQuery* query = new SQLQuery(); // consider making all pointers into smart pointers. reminder
	};
}

#endif // SQLProcessor.hpp