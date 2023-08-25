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
		selectTableStatement(UniqueDatabase& aDatabase, StatementType theType) : SQLStatement(aDatabase, theType) {}
		StatusResult  parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::noError;
			ParseHelper theHelper(aTokenizer);
			StringList theIdentifiers;
		
			aTokenizer.next();
			theResult = aTokenizer.skipIf(all) ? query->setAll() : theHelper.parseIdentifierList(theIdentifiers);
			if (theResult) {
				TableName theName;
				theResult = aTokenizer.skipTo(TokenType::identifier) ? theHelper.parseTableName(theName) : Errors::identifierExpected;
				query->getSchema()->setName(theName);
				if (theResult && isConditional(aTokenizer)) {
					theResult = query->getFilters().parse(aTokenizer, *query->getSchema());
				}
			}
			else
				theResult = Errors::invalidCommand;
			return theResult;
		}

		
	};
}
#endif // selectTableStatement.hpp
