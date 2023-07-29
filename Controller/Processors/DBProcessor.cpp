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
	DBProcessor::DBProcessor() {
		next = nullptr;
	}
	bool			DBProcessor::isProcessable(Keywords& aKeyword) const{
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
		return isProcessable(aTokenizer.current().keyword) ?
			   aTokenizer.peek().keyword == Keywords::database_kw || aTokenizer.peek().type == TokenType::identifier ? this : next->findHandler(aTokenizer) :
			   next->findHandler(aTokenizer);
	}
	Statement*		DBProcessor::makeStatement(Tokenizer& aTokenizer, AppProcessor* anAppProc) {
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
		return new DBStatement(anAppProc, theType);
	}
	StatusResult	DBProcessor::run(Statement* aStatement, ViewListener aViewer) {
		return Errors::notImplemented;
	}
}

