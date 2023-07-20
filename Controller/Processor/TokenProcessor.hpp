//
//  TokenProcessor.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef TokenProcessor_hpp
#define TokenProcessor_hpp

#include "../../Utilities/Tokenizer/Tokenizer.hpp"

namespace ECE141 {
	class TokenProcessor {
	public:
		TokenProcessor(Tokenizer& aTokenizer) : tokenizer(aTokenizer) {};
	protected:

		Tokenizer& tokenizer;
	};
}

#endif // TokenProcessor.hpp