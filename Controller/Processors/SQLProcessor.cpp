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
		case Keywords::update_kw:
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
					break;
				default:
					return aTokenizer.current().type == TokenType::identifier || isProcessable(aTokenizer.previous().keyword) ? this : nullptr;
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
			statement = (theType == StatementType::create) ? new createTableStatement(anAppController->getDB(), theType) : nullptr;
			break;
		case Keywords::insert_kw:
			theType = aTokenizer.skipTo(Keywords::into_kw) ? StatementType::insertTable : StatementType::unknown;
			statement = (theType == StatementType::insertTable) ? new insertTableStatement(anAppController->getDB(), theType) : nullptr;
			break;
		case Keywords::select_kw:
			theType = aTokenizer.skipTo(Keywords::from_kw) ? StatementType::selectTable : StatementType::unknown;
			statement = new selectTableStatement(anAppController->getDB(), theType);
			break;
		case Keywords::update_kw:
			theType = aTokenizer.skipTo(Keywords::set_kw) ? StatementType::updateTable : StatementType::unknown;
			statement = new updateTableStatement(anAppController->getDB(), theType);
			break;
		default:// drop, describe, show
			theType = Helpers::keywordToStmtType(theKeyword);
			statement = new SQLStatement(anAppController->getDB(), theType);
			break;
		}
		aTokenizer.restart();
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
	StatusResult		SQLProcessor::createTable(ViewListener aViewer) {
		/*StatusResult theResult = Errors::noError;
		auto* theDB = statement->getAppController()->getDB();
		if (!tables.count(statement->getSchema().getName())) {
			theResult =  theDB ? theDB->getStorage().add(BlockType::table_block, statement->getType(), statement->getQuery()) : Errors::noDatabaseSpecified;
			if (theResult) {
				StringView theView = "Query OK, 0 rows affected";
				aViewer(theView);
			}
		}
		return theResult;*/
		return Errors::notImplemented;
	}

	StatusResult		SQLProcessor::dropTable(ViewListener aViewer) {
		/*StatusResult theResult = Errors::noError;
		auto* theDB = statement->getAppController()->getDB();
		theResult = theDB ? theDB->getStorage().drop(BlockType::table_block, statement->getType(), statement->getQuery()) : Errors::noDatabaseSpecified;
		if (theResult) {
			StringView theView = "Query OK, 0 rows affected";
			aViewer(theView);
		}
		return theResult;
		*/
		return Errors::notImplemented;
	}

	StatusResult		SQLProcessor::describeTable(ViewListener aViewer){ 
		/*StatusResult theResult = Errors::noError;
		auto* theDB = statement->getAppController()->getDB();
		if (theDB) {
			TableOpt theTable = theDB->getTable(statement->getSchema()); // consider adding optional here.
			if (theTable.has_value()) {
				// describe show attributes
				// maybe table view?
			}
		}
		else
			theResult = Errors::noDatabaseSpecified;
		return theResult;*/
		return Errors::notImplemented;
	}
	StatusResult		SQLProcessor::showTables(ViewListener aViewer) {
		StatusResult theResult = Errors::noError;
		auto* theDB = statement->getDatabase();
		if (theDB) {
			std::stringstream theStream;
			size_t theTableLength = 0;
			size_t theLength = length + 4;

			theDB->fetchTables(tables);

			theStream << "+" << std::setfill('-') << std::setw(theLength) << "+\n";
			theStream << "| Database" << std::setfill(' ') << std::setw(theLength - 9) << "|\n";
			theStream << "+" << std::setfill('-') << std::setw(theLength) << "+\n";
			for (const auto& theTable : tables) {
				theTableLength = theTable.length();
				theStream << "| " << theTable << std::setfill(' ') << std::setw(theLength - 1 - theTableLength) << "|\n";
			}

			theStream << "+" << std::setfill('-') << std::setw(theLength) << "+\n";
			theStream << tables.size() << " rows in set";

			StringView theView(theStream.str());
			aViewer(theView);
		}
		else
			theResult = Errors::noDatabaseSpecified;
		return theResult;
	}
}