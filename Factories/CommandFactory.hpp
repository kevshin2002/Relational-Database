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

#include "../Commands/Command.hpp"
#include "../Utilities/Design/Creational/AbstractFactory.hpp"


namespace ECE141 {
	class CommandFactory : public Creational::AbstractFactory<CommandType, Command> {
	public:
		CommandFactory(Tokenizer& aTokenizer) : tokenizer(aTokenizer) {
			add<CommandType::basic_command>([]() { return Command(CommandType::basic_command); });
		};

		StatusResult transform() {
			while (tokenizer.more()) {
			}
		}
	protected:
		Tokenizer& tokenizer;
		Commands commands;
	};
}

#endif // CommandFactory.hpp