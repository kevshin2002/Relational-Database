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
	DBProcessor::DBProcessor() : next(new SQLProcessor()) { fetchDatabases(); }
	DBProcessor::~DBProcessor() {delete next;}
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
				switch (aTokenizer.current().keyword) {
					case Keywords::database_kw:
					case Keywords::databases_kw:
						break;
					default:
						return aTokenizer.current().type == TokenType::identifier ? this : next->findHandler(aTokenizer);
				}
			}
			return this;
		}
		return next->findHandler(aTokenizer);

	}
	Statement*		DBProcessor::makeStatement(Tokenizer& aTokenizer, AppController* anAppController) {
		aTokenizer.restart();
		Keywords theKeyword = aTokenizer.current().keyword;
		StatementType theType = StatementType::unknown;
		bool kwCheck = aTokenizer.skipTo(Keywords::database_kw);

		switch (theKeyword) {
		case Keywords::create_kw:
				theType = kwCheck ? StatementType::create : StatementType::unknown;
			break;
		case Keywords::drop_kw:
				theType = kwCheck ? StatementType::drop : StatementType::unknown;
			break;
		default:
			theType = Helpers::keywordToStmtType(theKeyword);
			break;
		}

		statement = new DBStatement(anAppController, theType);
		return statement;
	}
	StatusResult	DBProcessor::run(Statement* aStatement, ViewListener aViewer) {
		StatusResult theResult = Errors::noError;
		size_t theDBLength = statement->getDBName().length();

		if (theDBLength > length)
			length = theDBLength;
		switch (aStatement->getType()) {
		case StatementType::create:
			theResult = createDB(aViewer);
			break;
		case StatementType::drop:
			theResult = dropDB(aViewer);
			break;
		case StatementType::useDB:
			theResult = useDB(aViewer);
			break;
		case StatementType::dumpDB:
			theResult = dumpDB(aViewer);
			break;
		case StatementType::show:
			theResult = showDB(aViewer);
			break;
		default:
			break;
		}
		return theResult;
	}

	bool DBProcessor::fetchDatabases() {
		std::string thePath = Config::getStoragePath();
		std::string theExtension = Config::getDBExtension();
		std::string theFileName;
		size_t theLength = 0;

		for (const auto& theFile : std::filesystem::directory_iterator(thePath)) {
			if (theFile.path().extension().string() == theExtension) {
				theFileName = theFile.path().filename().replace_extension().string();
				databases.insert(theFileName);
				theLength = theFileName.length();
				if (theLength > length)
					length = theLength;
			}
		}
		return true;
	}

	StatusResult DBProcessor::createDB(ViewListener aViewer) {
		StatusResult theResult = Errors::databaseExists;
		std::string theDBName = statement->getDBName();

		if (!dbExists(theDBName)) {
			auto theDB = new Database(theDBName, CreateFile());
			databases.insert(theDBName);
			delete theDB;
			StringView theView = "Query OK, 1 row affected";
			aViewer(theView);
			theResult = Errors::noError;
		}
		return theResult;
	}
	StatusResult DBProcessor::dropDB(ViewListener aViewer) {
		StatusResult theResult = Errors::databaseDeletionError;
		std::string theDBName = statement->getDBName();
		AppController* theController = statement->getAppController();

		if (dbExists(theDBName)) {
			auto theDB = theController->getDB();
			if (theDB->inUse(theDBName))
				theController->releaseDB();
			fs::remove(Config::getDBPath(theDBName));
			databases.erase(theDBName);
			StringView theView = "Query OK, 0 row affected";
			aViewer(theView);
			theResult = Errors::noError;
		}
		return theResult;
	}
	StatusResult DBProcessor::useDB(ViewListener aViewer) {
		StatusResult theResult = Errors::unknownDatabase;
		std::string theDBName = statement->getDBName();

		if (dbExists(theDBName)) {
			auto theDB = new Database(theDBName, OpenFile());
			auto theAppController = statement->getAppController();
			theAppController->holdDB(theDB);
			StringView theView = "Database changed";
			aViewer(theView);
			theResult = Errors::noError;
		}
		return theResult;
	}

	StatusResult DBProcessor::showDB(ViewListener aListener) {
		std::stringstream theStream;
		size_t theDBLength = 0;
		size_t theLength = length + 4;

		theStream << "+" << std::setfill('-') << std::setw(theLength) << "+\n";
		theStream << "| Database" << std::setfill(' ') << std::setw(theLength - 9) << "|\n";
		theStream << "+" << std::setfill('-') << std::setw(theLength) << "+\n";

		for (const auto& theDB : databases) {
			theDBLength = theDB.length();
			theStream << "| " << theDB << std::setfill(' ') << std::setw(theLength - 1 - theDBLength) << "|\n";
		}

		theStream << "+" << std::setfill('-') << std::setw(theLength) << "+\n";
		theStream << databases.size() << " rows in set";

		StringView theView(theStream.str());
		aListener(theView);
		return Errors::noError;
		}

	StatusResult DBProcessor::dumpDB(ViewListener aViewer) {
		StatusResult theResult = Errors::noDatabaseSpecified;
		std::stringstream theStream;
		auto theController = statement->getAppController();
		std::string theDBName = statement->getDBName();

		if (dbExists(theDBName)) {
			auto theDB = new Database(theDBName, OpenFile());
			theController->holdDB(theDB);
			theResult = theDB->dump(theStream);
			StringView theView(theStream.str());
			aViewer(theView);
		}

		return theResult;
	}

}

