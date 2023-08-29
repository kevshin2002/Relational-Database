//
//  AppProcessor.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//


#ifndef AppProcessor_hpp
#define AppProcessor_hpp

#include "../../Statements/Statement.hpp"
#include "../../Database/Database.hpp"
#include "../../View/Views/StringView.hpp"

namespace ECE141 {
	class AppController;
	class AppProcessor {
	public:
		virtual ~AppProcessor(){}
		virtual bool				isProcessable(Keywords& aKeyword) const = 0;
		virtual AppProcessor*		findHandler(Tokenizer& aTokenizer) = 0;
		virtual Statement*			makeStatement(Tokenizer& aTokenizer, AppController* anAppController) = 0;
		virtual StatusResult		run(Statement* aStatement, ViewListener aViewer) = 0;
	};
}

#endif // AppProcessor.hpp