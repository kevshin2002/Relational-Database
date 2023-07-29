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
	  Statement*		makeStatement(Tokenizer& aTokenizer, AppProcessor* anAppProc) override;
	  StatusResult	    run(Statement* aStatement, ViewListener aViewer) override;

	protected:
	  AppProcessor* next;
	};
}


#endif // DBProcessor.hpp