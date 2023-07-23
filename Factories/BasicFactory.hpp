//
//  BasicFactory.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef BasicFactory_hpp
#define BasicFactory_hpp

#include "../Utilities/Design/Creational/AbstractFactory.hpp"
#include "../Statements/Statement.hpp"
#include "../Commands/Command.hpp"
#include "../Utilities/Helpers.hpp"

namespace ECE141 {
	class BasicFactory : public Creational::AbstractFactory<StatementType, UniqueStatement>{
	public:
		BasicFactory() {
			add<StatementType::about>([]() { return std::make_unique<Statement>(StatementType::about); });
			add<StatementType::version>([]() { return std::make_unique<Statement>(StatementType::version); });
			add<StatementType::help>([]() { return std::make_unique<Statement>(StatementType::help); });
			add<StatementType::quit>([]() { return std::make_unique<Statement>(StatementType::quit); }); // Necessary? Finishing for the sake of completeness
		}
		BasicFactory(const BasicFactory&) = delete;
		BasicFactory& operator=(const BasicFactory&) = delete;
		static BasicFactory& getInstance() {
			static BasicFactory theFactory;
			return theFactory;
		}

		UniqueStatement produce(StatusResult& aResult, Command& aCommand) {
			UniqueStatement theStatement;
			StatementType theType;
			aResult = Errors::invalidCommand;
			if (aCommand.getTokens().size() == static_cast<size_t>(Quantity::one)) {
				theType = Helpers::KWToStatement(aCommand.getCmdType());
				theStatement = create(theType);
				aResult = theType == StatementType::quit ? Errors::userTerminated : Errors::noError;
			}
			return theStatement;
		}
	};
}

#endif // BasicFactory.hpp