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
#include <memory>

namespace ECE141 {
	class Statement {
	public:
		Statement(StatementType aType) : type(aType) {}
		virtual ~Statement() {}
		virtual StatusResult execute() = 0;
	protected:
		StatementType type;
	};
	using UniqueStatement = std::unique_ptr<Statement>;
}

#endif // Statement.hpp