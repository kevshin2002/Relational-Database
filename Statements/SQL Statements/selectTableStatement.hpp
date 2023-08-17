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
		
			aTokenizer.next();
			theResult = aTokenizer.skipIf(all) ? query->setAll() : theHelper.parseIdentifierList(theIdentifiers);
			if (theResult) {
				TableName theName;
				theResult = aTokenizer.skipTo(TokenType::identifier) ? theHelper.parseTableName(theName) : Errors::identifierExpected;
				schema = theName.table;
				if (theResult && isConditional(aTokenizer)) {
					theResult = filter.parse(aTokenizer, schema);
				}
			}
			else
				theResult = Errors::invalidCommand;
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
				case Keywords::min_kw:
					return true;
				default:
					break;
				}
			}
			return false;
		}
	protected:
		Filters filter;
	};
}
#endif // selectTableStatement.hpp
