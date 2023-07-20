//
//  StatementFactory.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef StatementFactory_hpp
#define StatementFactory_hpp

#include "../Commands/Command.hpp"
#include "../Statements/Statement.hpp"
#include "BasicFactory.hpp"
#include <functional>

namespace ECE141 {
	
	class StatementRouter {
	public:
		StatementRouter(const StatementRouter&) = delete;
		StatementRouter& operator=(const StatementRouter&) = delete;
		static StatementRouter& getInstance() {
			static StatementRouter theRouter;
			return theRouter;
		}

		UniqueStatement route(Command& aCommand) {
			auto callable = routes.find(aCommand.getType());
			return callable != routes.end() ? callable->second(aCommand.getTokens()) : nullptr;
		}

	protected:
		using RouteCalls = std::map <CommandType, std::function<UniqueStatement(std::vector<Token>&)>>;
		RouteCalls routes {
			{CommandType::basic_command, [](std::vector<Token>& aTokenList) { return BasicFactory::getInstance().factorize(aTokenList); }}
		};
	private:
		StatementRouter() {}
	};
}

#endif // StatementFactory.hpp