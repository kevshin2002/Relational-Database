//
//  updateTableStatement.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef updateTableStatement_hpp
#define updateTableStatement_hpp

#include "SQLStatement.hpp"

namespace ECE141 {
	class updateTableStatement : public SQLStatement {
	public:
		updateTableStatement(UniqueDatabase& aDatabase, StatementType theType) : SQLStatement(aDatabase, theType) {}
		StatusResult parse(Tokenizer& aTokenizer) override {
			StatusResult theResult = Errors::noError;
			ParseHelper theHelper(aTokenizer);
			TableName theName;

			theResult = aTokenizer.skipTo(TokenType::identifier) ? theHelper.parseTableName(theName) : Errors::identifierExpected;
			theResult = aTokenizer.skipIf(Keywords::set_kw) ? query->getFilters().parse(aTokenizer, *query->getSchema()) : Errors::setKeywordExpected;
			return theResult;
		}
	};
}

#endif/