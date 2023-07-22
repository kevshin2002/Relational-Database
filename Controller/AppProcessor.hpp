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

namespace ECE141 {
	struct AppProcessor {
		StatusResult virtual process(Statement& aStatement) = 0;
	};
}



#endif // ErrorProcessor_hpp