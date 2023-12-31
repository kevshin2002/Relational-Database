//
//  SQLProcessor.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef SQLProcessor_hpp
#define SQLProcessor_hpp

#include "AppProcessor.hpp"
#include "../../Statements/SQL Statements/createTableStatement.hpp"
#include "../../Statements/SQL Statements/insertTableStatement.hpp"
#include "../../Statements/SQL Statements/selectTableStatement.hpp"
#include "../../Statements/SQL Statements/updateTableStatement.hpp"
#include "../../Utilities/Validator.hpp"

namespace ECE141 {
	class SQLProcessor : public AppProcessor {
	public:
	SQLProcessor();
	~SQLProcessor();

	bool				isProcessable(Keywords& aKeyword) const override;
	AppProcessor*		findHandler(Tokenizer& aTokenizer) override;
	Statement*			makeStatement(Tokenizer& aTokenizer, AppController* anAppController) override;
	StatusResult	    run(Statement* aStatement, ViewListener aViewer) override;

	protected:
    StatusResult process(StatementType aType, DBQuery* aQuery);
	StatusResult initialize();
	StatusResult		createTable(ViewListener aViewer);
	StatusResult		dropTable(ViewListener aViewer);
	StatusResult		insertTable(ViewListener aViewer);
	StatusResult		selectTable(ViewListener aViewer);
	StatusResult		updateTable(ViewListener aViewer);
	StatusResult		describeTable(ViewListener aViewer);
	StatusResult		showTables(ViewListener aViewer);

	private:
	// This is populated through the API in Storage (fetchTables).
	// It is ran in the beginning of run only when the size of the table is zero.
	std::set<std::string> tables;
	std::string name = "none";

	SQLStatement* statement = nullptr;
	AppProcessor* next = nullptr;

	// Members for formatting.
	std::stringstream stream;
	size_t length = 8;

	};
}

#endif // SQLProcessor.hpp