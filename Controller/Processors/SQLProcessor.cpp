//
//  SQLProcessor.cpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#include "SQLProcessor.hpp"

namespace ECE141 {
	SQLProcessor::SQLProcessor() {}
	SQLProcessor::~SQLProcessor() {
		if (next)
			delete next;
	}
	bool	SQLProcessor::isProcessable(Keywords& aKeyword) const {
		switch (aKeyword) {
		case Keywords::create_kw:
		case Keywords::drop_kw:
		case Keywords::describe_kw:
		case Keywords::show_kw:
			return true;
		default:
			return false;
		}
	}
	AppProcessor* SQLProcessor::findHandler(Tokenizer&aTokenizer) {
		aTokenizer.restart();
		if (isProcessable(aTokenizer.current().keyword)) {
			aTokenizer.next();
			if (aTokenizer.more()) {
				switch (aTokenizer.current().keyword) {
				case Keywords::table_kw:
				case Keywords::tables_kw:
					break;
				default:
					return aTokenizer.current().type == TokenType::identifier ? this : nullptr;
				}
			}
			return this;
		}
		return nullptr;
	}
	Statement* SQLProcessor::makeStatement(Tokenizer& aTokenizer, AppController* anAppController) {
		aTokenizer.restart();
		Keywords theKeyword = aTokenizer.current().keyword;
		StatementType theType = StatementType::unknown;
		aTokenizer.skipTo(Keywords::table_kw);

		switch (theKeyword) {
		case Keywords::create_kw:
			if (aTokenizer.more())
				theType = aTokenizer.current().keyword == Keywords::table_kw ? StatementType::create : StatementType::unknown;
			break;
		case Keywords::drop_kw:
			if (aTokenizer.more())
				theType = aTokenizer.current().keyword == Keywords::table_kw ? StatementType::drop : StatementType::unknown;
			break;
		default:
			theType = Helpers::keywordToStmtType(theKeyword);
			break;
		}
		statement = new SQLStatement(anAppController, theType);
		return statement;
	}
	StatusResult	    SQLProcessor::run(Statement* aStatement, ViewListener aViewer) {
		StatusResult theResult = Errors::noError;

		switch (aStatement->getType()) {
		case StatementType::create:
			theResult = createTable(aViewer);
			break;
		case StatementType::drop:
			theResult = dropTable(aViewer);
			break;
		case StatementType::describeTable:
			theResult = describeTable(aViewer);
			break;
		case StatementType::show:
			theResult = showTables(aViewer);
			break;
		default:
			break;
		}
		return theResult;
	}
	StatusResult		SQLProcessor::createTable(ViewListener aViewer){ return Errors::notImplemented; }
	StatusResult		SQLProcessor::dropTable(ViewListener aViewer){ return Errors::notImplemented; }
	StatusResult		SQLProcessor::describeTable(ViewListener aViewer){ return Errors::notImplemented; }
	StatusResult		SQLProcessor::showTables(ViewListener aViewer) { return Errors::notImplemented; }
}