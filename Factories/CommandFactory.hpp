//
//  CommandFactory.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef CommandFactory_hpp
#define CommandFactory_hpp

#include "../Utilities/Design/Creational/AbstractFactory.hpp"
#include "../Utilities/Parser.hpp"


namespace ECE141 {
	class CommandFactory : public Creational::AbstractFactory<CommandType, Command, std::vector<Token>&> {
	public:
		CommandFactory(Tokenizer& aTokenizer) : tokenizer(aTokenizer) {
			add<CommandType::basic_command>([&](std::vector<Token>& aTokenList) { return Command(CommandType::basic_command, aTokenList); });
		};

		/*
		@package.first = CommandType
		@package.second = std::vector<Token>
		*/

		CommandFactory& factorize() {
			while (tokenizer.more()) {
				CmdCreate package = Parser::parse(tokenizer.getTokens());
				Command theCmd = create(package.first, package.second);
				commands.push_back(theCmd);
			}
			return *this;
		}

		const size_t more() {
			return commands.size();
		}

		Command& getCmd() { return commands.front(); }
		void deleteCmds(Quantity aQnty) {
			aQnty != Quantity::all ? commands.erase(commands.begin() + (static_cast<size_t>(aQnty) - 1)) : commands.clear();
		}
	protected:

		Tokenizer& tokenizer;
		Commands commands;
	
	};
}

#endif // CommandFactory.hpp