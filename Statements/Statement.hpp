//
//  Statement.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef Statement_hpp
#define Statement_hpp

#include "../Misc/Types/keywords.hpp"

namespace ECE141 {
	class Statement {
	public:
		virtual ~Statement() {}
		virtual StatusResult execute() = 0;
	};
	using UniqueStatement = std::unique_ptr<Statement>;
}

#endif // Statement.hpp