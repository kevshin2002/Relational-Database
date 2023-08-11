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
		case Keywords::insert_kw:
		case Keywords::select_kw:
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
				case Keywords::into_kw:
				case Keywords::from_kw:
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
		switch (theKeyword) {
		case Keywords::create_kw:
			theType = aTokenizer.skipTo(Keywords::table_kw) ? StatementType::create : StatementType::unknown;
			statement = (theType == StatementType::create) ? new createTableStatement(anAppController, theType) : nullptr;
			break;
		case Keywords::insert_kw:
			theType = aTokenizer.skipTo(Keywords::into_kw) ? StatementType::insertTable : StatementType::unknown;
			statement = (theType == StatementType::insertTable) ? new insertTableStatement(anAppController, theType) : nullptr;
			break;
		case Keywords::select_kw:
			statement = new selectTableStatement(anAppController, StatementType::selectTable);
			break;
		default:// drop, describe, show
			theType = Helpers::keywordToStmtType(theKeyword);
			statement = new SQLStatement(anAppController, theType);
			break;
		}
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