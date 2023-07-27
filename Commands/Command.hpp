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
	struct Command {
		Command() : type(CommandType::unknown_command) {}
		Command(CommandType aCmdType, const std::vector<Token>& aTokenList) : type(aCmdType), tokens(aTokenList) {}

		virtual ~Command() {}
		CommandType& getType() { return type; }
		Keywords& getCmdType() { return tokens.front().keyword; }
		std::vector<Token>& getTokens() { return tokens; }

		CommandType type;
		std::vector<Token> tokens;
	};
	using CommandsVec = std::vector<Command>;
	using CmdCreate = std::pair<CommandType, std::vector<Token>>;
}

#endif // Command.hpp