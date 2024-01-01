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
		StatusResult theResult = statement->getDatabase()->inUse() ? initialize() : Errors::noDatabaseSpecified;
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
		if (!tables.count(theQuery->getSchema()->getName())){
			theResult = theDB ? theDB->getStorage().add(statement->getType(), theQuery) : Errors::noDatabaseSpecified;
			if (theResult) {
				tables.insert(theQuery->getSchema()->getName());
				StringView theView = "Query OK, 1 rows affected";
				aViewer(theView);
			}
		}
		return theResult;
	}

	StatusResult		SQLProcessor::dropTable(ViewListener aViewer) {
        StatusResult theResult = Errors::noError;
		auto& theDB = statement->getDatabase();
		auto& theTableName = statement->getIdentifier();
		if (tables.count(theTableName)) {
			theResult = theDB ? theDB->getStorage().drop(theTableName, statement->getQuery()) : Errors::noDatabaseSpecified;
			tables.erase(tables.find(theTableName));
			if (theResult) {
				StringView theView = "Query OK, 0 rows affected";
				aViewer(theView);
			}
		}
		return theResult;
	}

	StatusResult	SQLProcessor::insertTable(ViewListener aViewer) {
		StatusResult theResult = Errors::unknownTable;
		auto& theDB = statement->getDatabase();
		auto* theQuery = statement->getQuery();
		if (tables.count(theQuery->getSchema()->getName())) {
			if (theResult = process(statement->getType(), theQuery)) {
				theResult = theDB ? theDB->getStorage().add(statement->getType(), theQuery) : Errors::noDatabaseSpecified;
				if (theResult) {
					std::stringstream theStream;
					theStream << "Query Ok, " << theQuery->getRows().size() << " rows affected";
					StringView theView(theStream.str());
					aViewer(theView);
				}
			}
		}
		return theResult;
	}

	StatusResult	SQLProcessor::selectTable(ViewListener aViewer) {
		StatusResult theResult = Errors::unknownTable;
		auto& theDB = statement->getDatabase();
		auto* theQuery = statement->getQuery();
		auto theName = theQuery->getSchema()->getName();
		if (tables.count(theName)) {
			theResult = theDB ? theDB->getStorage().read(statement->getType(), theQuery) : Errors::noDatabaseSpecified;
			if (theResult) {
				std::stringstream theStream;
				theStream << "Query Ok, " << theDB->getStorage().getRows(theName).size() << " rows affected";
				StringView theView(theStream.str());
				aViewer(theView);
			}
		}
		return theResult;
	}

	StatusResult	SQLProcessor::updateTable(ViewListener aViewer) {
		return Errors::notImplemented;
	}
	StatusResult		SQLProcessor::describeTable(ViewListener aViewer){ 
		std::string theName = statement->getIdentifier();
		StatusResult theResult = theName.size() ? Errors::noError : Errors::unknownTable;
		// fix case sensitive
		// Consider maybe OxE x OxI, a string formatter of some sort?
		// repetitive formatting
		if (theResult) {
			auto& theDB = statement->getDatabase();
			Schema* theSchema = theDB->getStorage().getSchema(theName);
			AttributeList& theAttributes = theSchema->getAttributes();
			std::stringstream theStream;
			size_t theAttributeLength = 0;
			size_t theTypeLength = std::max_element(theAttributes.begin(), theAttributes.end(),
				[](const Attribute& aAttribute_One, const Attribute& aAttribute_Two) {
					return strlen(Helpers::dataTypeToString(aAttribute_One.getType())) < strlen(Helpers::dataTypeToString(aAttribute_Two.getType()));
				})->getName().size() + 10;
				
			size_t theFieldLength = std::max_element(theAttributes.begin(), theAttributes.end(),
				[](const Attribute& aAttribute_One, const Attribute& aAttribute_Two) {
					return aAttribute_One.getName().size() < aAttribute_Two.getName().size();
				})->getName().size() + 10;


			theStream << "+" << std::setfill('-') << std::setw(theFieldLength);
			theStream << "+" << std::setfill('-') << std::setw(theTypeLength - 1);
			theStream << "+" << std::setfill('-') << std::setw(7);
			theStream << "+" << std::setfill('-') << std::setw(6);
			theStream << "+" << std::setfill('-') << std::setw(11);
			theStream << "+" << std::setfill('-') << std::setw(20) << "+\n";


			theStream << "| Field " << std::setfill(' ') << std::setw(theFieldLength - 7) << "|";
			theStream << " Type " << std::setfill(' ') << std::setw(theTypeLength - 7) << "|";
			theStream << " Null " << "|";
			theStream << " Key " << "|";
			theStream << " Default " << std::setfill(' ') << std::setw(2) << "|";
			theStream << " Extra " << std::setfill(' ') << std::setw(13) << "|\n";


			theStream << "+" << std::setfill('-') << std::setw(theFieldLength);
			theStream << "+" << std::setfill('-') << std::setw(theTypeLength - 1);
			theStream << "+" << std::setfill('-') << std::setw(7);
			theStream << "+" << std::setfill('-') << std::setw(6);
			theStream << "+" << std::setfill('-') << std::setw(11);
			theStream << "+" << std::setfill('-') << std::setw(20) << "+\n";

			for (const auto& theAttribute : theAttributes) {
				theAttributeLength = theAttribute.getName().size();
				theStream << "| " << theAttribute.getName() << std::setfill(' ') << std::setw(theFieldLength - 1 - theAttributeLength) << " |";
			
				std::string theValue = Helpers::dataTypeToString(theAttribute.getType());
				theAttributeLength = strlen(Helpers::dataTypeToString(theAttribute.getType()));
				if (theAttribute.getType() == DataTypes::varchar_type) {
					theValue += "(" + std::to_string(theAttribute.getSize()) + ")";
					theAttributeLength += theValue.length() - theAttributeLength;
				}
				theStream << " " << theValue << std::setfill(' ') << std::setw(theTypeLength - 2 - theAttributeLength) << "|";
				theValue = theAttribute.isNull() ? "YES" : "NO";
				theStream << " " << theValue << std::setfill(' ') << std::setw(7 - 1 - theValue.length()) <<	" |";

				theValue = theAttribute.isPrimary() ? "PRI" : " ";
				theStream << " " << theValue << std::setfill(' ') << std::setw(6 - 1 - theValue.length()) << " |";

				theValue = theAttribute.isNull() ? " " : "NULL";
				theStream << " " << theValue << std::setfill(' ') << std::setw(11 - 1 - theValue.length()) << " |";

				theValue = theAttribute.isIncrement() ? "auto_increment" : " ";
				theStream << " " << theValue << std::setfill(' ') << std::setw(20 - 1 - theValue.length()) << " |\n";
			}

			theStream << "+" << std::setfill('-') << std::setw(theFieldLength);
			theStream << "+" << std::setfill('-') << std::setw(theTypeLength - 1);
			theStream << "+" << std::setfill('-') << std::setw(7);
			theStream << "+" << std::setfill('-') << std::setw(6);
			theStream << "+" << std::setfill('-') << std::setw(11);
			theStream << "+" << std::setfill('-') << std::setw(20) << "+\n";
			theStream << theAttributes.size() << " rows in set";

			StringView theView(theStream.str());
			aViewer(theView);
		}
		return theResult;
	}
	StatusResult		SQLProcessor::showTables(ViewListener aViewer) {
		StatusResult theResult = Errors::noError;
		auto& theDB = statement->getDatabase();

		if (theDB.get()) {
			std::stringstream theStream;
			size_t theTableLength = 0;
			size_t theLength = length + 5; // consider using max_element

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
		AttributeList& theAttributes = theDBSchema->getAttributes();
		StatusResult theResult = Errors::noError;

		auto theIdentifiers = aQuery->getIdentifiers();
		auto theValues = aQuery->getValues();

		switch (aType) {
		case StatementType::insertTable:
			aQuery->setRows(Validator::insert(theDBSchema->getHash(), theAttributes, theIdentifiers, theValues, theResult));
			break;
		default:
			break;
		}


		return Errors::noError;
	}
	// Fix
	StatusResult SQLProcessor::initialize() {
		StatusResult theResult = Errors::noError;
		if (name != statement->getDatabase()->getName()) {
			tables = std::set<std::string>();
			statement->getDatabase()->fetchTables(tables);
			name = statement->getDatabase()->getName();
			if (tables.size()) {
				length = std::max_element(tables.begin(), tables.end(),
					[](const std::string& aTable_One, const std::string& aTable_Two) {
						return aTable_One.size() < aTable_Two.size();
					}
				)->size();
			}
		}
		return theResult;
	}
}