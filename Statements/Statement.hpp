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
		virtual ~Statement() {}
		StatusResult virtual parse(Tokenizer& aTokenizer) = 0;
	protected:
		StatementType type;
	};
}

#endif // Statement.hpp