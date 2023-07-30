//
//  DBProcessor.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//


#ifndef DBProcessor_hpp
#define DBProcessor_hpp


#include "AppProcessor.hpp"
#include "../../Statements/DBStatement.hpp"

namespace ECE141 {
	class DBProcessor : public AppProcessor {
	public:
	  DBProcessor();
	  ~DBProcessor();
	  bool				isProcessable(Keywords& aKeyword) const override;
	  AppProcessor*		findHandler(Tokenizer& aTokenizer) override;
	  Statement*		makeStatement(Tokenizer& aTokenizer, AppController* anAppController) override;
	  StatusResult	    run(Statement* aStatement, ViewListener aViewer) override;

	protected:
		StatusResult createDB(ViewListener aViewer);
		StatusResult dropDB(ViewListener aViewer);
		StatusResult showDB(ViewListener aViewer);
		StatusResult useDB(ViewListener aViewer);
		StatusResult dumpDB(ViewListener aViewer);

		AppProcessor* next = nullptr;
	};
}


#endif // DBProcessor.hpp