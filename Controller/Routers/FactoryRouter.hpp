//
//  FactoryRouter.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef FactoryRouter_hpp
#define FactoryRouter_hpp

#include "../../Commands/Command.hpp"
#include "../../Statements/Statement.hpp"
#include "../../Factories/BasicFactory.hpp"
#include <functional>

namespace ECE141 {
	class FactoryRouter {
	public:
		UniqueStatement route(StatusResult& aResult, Command& aCommand) {
			auto callable = routes.find(aCommand.getType());
			return callable != routes.end() ? callable->second(aResult, aCommand) : nullptr;
		}

	protected:
		using RouteCalls = std::map <CommandType, std::function<UniqueStatement(StatusResult&, Command&)>>;
		RouteCalls routes {
			{CommandType::basic_command, [](StatusResult& aResult, Command& aCommand) { return BasicFactory::getInstance().produce(aResult, aCommand); }} 
		};

		// What if we add more types of commands?
		// We don't want to contain multiple factories and should just route (SRP).
	};

}

#endif // FactoryRouter.hpp