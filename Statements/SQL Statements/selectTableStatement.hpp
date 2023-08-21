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
		selectTableStatement(Database* aDatabase, StatementType theType) : SQLStatement(aDatabase, theType) {}
		StatusResult  parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::noError;
			ParseHelper theHelper(aTokenizer);
			StringList theIdentifiers;
		
			aTokenizer.next();
			theResult = aTokenizer.skipIf(all) ? query->setAll() : theHelper.parseIdentifierList(theIdentifiers);
			if (theResult) {
				TableName theName;
				theResult = aTokenizer.skipTo(TokenType::identifier) ? theHelper.parseTableName(theName) : Errors::identifierExpected;
				//schema = theName.table;
				if (theResult && isConditional(aTokenizer)) {
					//theResult = query->getFilters().parse(aTokenizer, schema);
				}
			}
			else
				theResult = Errors::invalidCommand;
			return theResult;
		}

		
	};
}
#endif // selectTableStatement.hpp
