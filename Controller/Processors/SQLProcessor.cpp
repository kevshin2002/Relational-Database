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
		statement = nullptr;
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
		auto& theDB = anAppController->getDB();
		StatementType theType = StatementType::unknown;
		switch (theKeyword) {
		case Keywords::create_kw:
			theType = aTokenizer.skipTo(Keywords::table_kw) ? StatementType::create : StatementType::unknown;
			statement = (theType == StatementType::create) ? new createTableStatement(theDB, theType) : nullptr;
			break;
		case Keywords::insert_kw:
			theType = aTokenizer.skipTo(Keywords::into_kw) ? StatementType::insertTable : StatementType::unknown;
			statement = (theType == StatementType::insertTable) ? new insertTableStatement(theDB, theType) : nullptr;
			break;
		case Keywords::select_kw:
			theType = aTokenizer.skipTo(Keywords::from_kw) ? StatementType::selectTable : StatementType::unknown;
			statement = new selectTableStatement(theDB, theType);
			break;
		case Keywords::update_kw:
			theType = aTokenizer.skipTo(Keywords::set_kw) ? StatementType::updateTable : StatementType::unknown;
			statement = new updateTableStatement(theDB, theType);
			break;
		default:// drop, describe, show
			theType = Helpers::keywordToStmtType(theKeyword);
			statement = new SQLStatement(theDB, theType);
			break;
		}
		aTokenizer.restart();
		return statement;
	}

	// insert, select, update,
	// 
	StatusResult	    SQLProcessor::run(Statement* aStatement, ViewListener aViewer) {
		StatusResult theResult = Errors::noError;
		tables = std::set<std::string>{};
		statement->getDatabase()->fetchTables(tables);
		if (theResult) {
			switch (aStatement->getType()) {
			case StatementType::create:
				theResult = createTable(aViewer);
				break;
			case StatementType::drop:
				theResult = dropTable(aViewer);
				break;
			case StatementType::insertTable:
				theResult = insertTable(aViewer);
				break;
			case StatementType::selectTable:
				theResult = selectTable(aViewer);
				break;
			case StatementType::updateTable:
				theResult = updateTable(aViewer);
			case StatementType::describeTable:
				theResult = describeTable(aViewer);
				break;
			case StatementType::show:
				theResult = showTables(aViewer);
				break;
			default:
				break;
			}
		}
		return theResult;
	}
	StatusResult		SQLProcessor::createTable(ViewListener aViewer) {
		StatusResult theResult = Errors::tableExists;
		auto& theDB = statement->getDatabase();
		auto* theQuery = statement->getQuery();
		if (!tables.count(theQuery->getSchema()->getName()))
			theResult = theDB ? theDB->getStorage().add(statement->getType(), theQuery) : Errors::noDatabaseSpecified;
	
		if (theResult) {
			StringView theView = "Query OK, 0 rows affected";
			aViewer(theView);
		}
		return theResult;
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

	StatusResult	SQLProcessor::insertTable(ViewListener aViewer) {
		StatusResult theResult = Errors::unknownTable;
		auto& theDB = statement->getDatabase();
		auto* theQuery = statement->getQuery();
		if (tables.count(theQuery->getSchema()->getName())) {
			if (theResult = process(statement->getType(), theQuery)) {
				theResult = theDB ? theDB->getStorage().add(statement->getType(), theQuery) : Errors::noDatabaseSpecified;
			}
		}
		if (theResult) {
			std::stringstream theStream;
			theStream << "Query Ok, " << theQuery->getRows().size() << " rows affected";
			StringView theView(theStream.str());
			aViewer(theView);
		}
		return theResult;
	}

	StatusResult	SQLProcessor::selectTable(ViewListener aViewer) {
		return Errors::notImplemented;
	}

	StatusResult	SQLProcessor::updateTable(ViewListener aViewer) {
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
		auto& theDB = statement->getDatabase();
		if (theDB.get()) {
			std::stringstream theStream;
			size_t theTableLength = 0;
			size_t theLength = length + 5;

			theStream << "+" << std::setfill('-') << std::setw(theLength) << "+\n";
			theStream << "| Tables" << std::setfill(' ') << std::setw(theLength - 7) << "|\n";
			theStream << "+" << std::setfill('-') << std::setw(theLength) << "+\n";
			for (const auto& theTable : tables) {
				theTableLength = theTable.length();
				theStream << "| " << theTable << std::setfill(' ') << std::setw(theLength - 1 - theTableLength) << "|\n";
			}

			theStream << "+" << std::setfill('-') << std::setw(theLength) << "+\n";
			theStream << tables.size() << " rows in set";

			StringView theView(theStream.str());
			aViewer(theView);
			return Errors::noError;
		}
		else
			theResult = Errors::noDatabaseSpecified;
		return theResult;
	}

	StatusResult SQLProcessor::process(StatementType aType, DBQuery* aQuery) {
		auto theName = aQuery->getSchema()->getName();
		auto theDBSchema = statement->getDatabase()->getSchema(theName);
		auto theTable = statement->getDatabase()->getTable(theName);
		AttributeList& theAttributes = theDBSchema->getAttributes();
		StatusResult theResult = Errors::noError;

		auto theIdentifiers = aQuery->getIdentifiers();
		auto theValues = aQuery->getValues();

		switch (aType) {
		case StatementType::insertTable:
			aQuery->setRows(Validator::insert(theAttributes, theIdentifiers, theValues, theResult));
			break;
		case StatementType::selectTable:
			break;
		default:
			break;
		}


		return Errors::noError;
	}
}