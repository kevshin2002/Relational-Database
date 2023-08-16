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
			StatusResult theResult = Errors::noError;
			ParseHelper theHelper(aTokenizer);
			StringList theIdentifiers;
			TableName theName;

			aTokenizer.next();
			theResult = aTokenizer.skipIf(all) ? query->setAll() : theHelper.parseIdentifierList(theIdentifiers);
			if (theResult && aTokenizer.skipIf(Keywords::from_kw)) {
				theResult = aTokenizer.skipTo(TokenType::identifier) ? theHelper.parseTableName(theName) : Errors::identifierExpected;
				schema = theName.table;
				if (theResult && aTokenizer.skipIf(Keywords::where_kw)) {
					theResult = filter.parse(aTokenizer, schema);
				}
			}
			else
				theResult = Errors::invalidCommand;
			return theResult;
		}

	protected:
		Filters filter;
	};
}
#endif // selectTableStatement.hpp
