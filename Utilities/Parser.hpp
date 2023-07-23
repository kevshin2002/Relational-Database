//
//  BasicFactory.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef Parser_hpp
#define Parser_hpp

#include "Helpers.hpp"
#include "../Commands/Command.hpp"

namespace ECE141 {
	struct Parser {
		static CmdCreate parse(std::vector<Token>& aTokenList) {
            std::vector<Token> theCmdTokens;
            auto iterator = aTokenList.begin();
            CommandType theType = Helpers::KWToCmd(iterator->keyword);
            theCmdTokens.push_back(*iterator);
            iterator = aTokenList.erase(iterator);
            while (iterator != aTokenList.end()) {
                if (Helpers::isCmd(iterator->keyword))
                    break;
                theCmdTokens.push_back(*iterator);
                iterator = aTokenList.erase(iterator);
            }
			return std::make_pair(theType, theCmdTokens);
		}
	};
}

#endif // Parser.hpp