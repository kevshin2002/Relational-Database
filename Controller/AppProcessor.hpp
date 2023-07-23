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

#include "../Statements/Statement.hpp"
#include "../View/ViewGenerator.hpp"

namespace ECE141 {
	struct AppProcessor {
		UniqueView virtual process(UniqueStatement& aStatement) = 0;
	};
	using UniqueAppProc = std::unique_ptr<AppProcessor>;
}



#endif // AppProcessor_hpp