//
//  DBProcessor.cpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//


#include "DBProcessor.hpp"

namespace ECE141 {
	DBProcessor::DBProcessor() {}
	DBProcessor::~DBProcessor() {
		if (next)
			delete next;
	}
	bool	DBProcessor::isProcessable(Keywords& aKeyword) const{
		switch (aKeyword) {
		case Keywords::create_kw:
		case Keywords::show_kw:
		case Keywords::use_kw:
		case Keywords::drop_kw:
		case Keywords::dump_kw:
			return true;
		default:
			return false;
		}
	}
	
	AppProcessor*	DBProcessor::findHandler(Tokenizer& aTokenizer) {
		if (isProcessable(aTokenizer.current().keyword)) {
			aTokenizer.next();
			if (aTokenizer.more()) {
				return aTokenizer.current().keyword == Keywords::database_kw || aTokenizer.current().type == TokenType::identifier ?
					   this : 
					   next->findHandler(aTokenizer);
			}
		}
		return nullptr;

	}
	Statement*		DBProcessor::makeStatement(Tokenizer& aTokenizer, AppController* anAppController) {
		aTokenizer.restart();
		Keywords theKeyword = aTokenizer.current().keyword;
		StatementType theType;
		aTokenizer.next();
		switch (theKeyword) {
		case Keywords::create_kw:
			theType = aTokenizer.current().keyword == Keywords::database_kw ? StatementType::createDB : StatementType::unknown;
			break;
		case Keywords::drop_kw:
			theType = aTokenizer.current().keyword == Keywords::database_kw ? StatementType::dropDB : StatementType::unknown;
			break;
		default:
			theType = Helpers::keywordToStmtType(theKeyword);
			break;
		}
		return new DBStatement(anAppController, theType);
	}
	StatusResult	DBProcessor::run(Statement* aStatement, ViewListener aViewer) {
		StatusResult theResult = Errors::noError;
		switch (aStatement->getType()) {
		case StatementType::createDB:
			theResult = createDB(aViewer);
			break;
		case StatementType::dropDB:
			theResult = dropDB(aViewer);
			break;
		case StatementType::showDB:
			theResult = showDB(aViewer);
			break;
		case StatementType::useDB:
			theResult = useDB(aViewer);
			break;
		case StatementType::dumpDB:
			theResult = dumpDB(aViewer);
			break;
		}
		return theResult;
	}

	StatusResult DBProcessor::createDB(ViewListener aViewer) {
		return Errors::notImplemented;
	}

	StatusResult DBProcessor::dropDB(ViewListener aViewer) {
		return Errors::notImplemented;
	}

	StatusResult DBProcessor::showDB(ViewListener aViewer) {
		return Errors::notImplemented;
	}

	StatusResult DBProcessor::useDB(ViewListener aViewer) {
		return Errors::notImplemented;
	}

	StatusResult DBProcessor::dumpDB(ViewListener aViewer) {
		return Errors::notImplemented;
	}
}

