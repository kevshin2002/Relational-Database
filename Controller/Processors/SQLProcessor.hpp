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
#include "../../Statements/SQLStatement.hpp"
#include "../../Database/Storage/Storage.hpp"

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
	StatusResult		createTable(ViewListener aViewer);
	StatusResult		dropTable(ViewListener aViewer);
	StatusResult		describeTable(ViewListener aViewer);
	StatusResult		showTables(ViewListener aViewer);

	private:
	std::set<std::string> tables;

	SQLStatement* statement = nullptr;
	AppProcessor* next = nullptr;

	// Members for formatting.
	std::stringstream stream;
	size_t length = 11;
	};
}

#endif // SQLProcessor.hpp