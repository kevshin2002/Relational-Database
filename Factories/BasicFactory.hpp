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

#include "../Utilities/Design/Creational/Abstractfactory.hpp"

namespace ECE141 {
	enum class BasicType {
		about = 'a', version = 'v', help = 'h', quit = 'q'
	};

	class BasicFactory : public Creational::AbstractFactory<BasicType, UniqueStatement>{
	public:
		BasicFactory(const BasicFactory&) = delete;
		BasicFactory& operator=(const BasicFactory&) = delete;
		static BasicFactory& getInstance() {
			static BasicFactory theFactory;
			return theFactory;
		}

		UniqueStatement factorize(std::vector<Token>& aTokenList) {
			return nullptr;
		}
	protected:

	private:
		BasicFactory() {
		add<BasicType::about>([]() { return  });
		}
	};
}

#endif // BasicFactory.hpp