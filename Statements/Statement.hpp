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

#include "../Utilities/Tokenizer/Tokenizer.hpp"
#include "../Utilities/FolderReader.hpp"
#include <memory>

namespace ECE141 {
	class Statement {
	public:
		Statement(StatementType aType) : type(aType) {}
		virtual ~Statement() {}
		virtual StatusResult	     parse(Tokenizer& aTokenizer) {return Errors::noError;}
		StatementType                getType() const { return type; }
		
	protected:
		StatementType type;
	};
}

#endif // Statement.hpp