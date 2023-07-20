//
//  Command.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef Command_hpp
#define Command_hpp

#include "../Utilities/Tokenizer/Tokenizer.hpp"

namespace ECE141 {
	class Command {
	public:
		virtual ~Command() {}
		CommandType& getType() { return type; }
		std::vector<Token>& getTokens() { return tokens; }

	protected:
		CommandType type;
		std::vector<Token> tokens;
	};

}

#endif // Command.hpp