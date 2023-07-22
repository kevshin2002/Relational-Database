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
	struct CmdQuery {
		Token marker;
		std::vector<Token> tokens;
	};
	struct Command {
		Command(CommandType aCmdType) : type(aCmdType) {}

		virtual ~Command() {}
		CommandType& getType() { return type; }
		Token& getMarker() { return query.marker; }
		std::vector<Token>& getTokens() { return query.tokens; }

		CommandType type;
		CmdQuery query;
	};
	using Commands = std::vector<Command>;

}

#endif // Command.hpp