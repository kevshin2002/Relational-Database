//
//  BasicProcessor.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef BasicProcessor_hpp
#define BasicProcessor_hpp

#include "../../AppProcessor.hpp"

namespace ECE141 {
	class BasicProcessor : public AppProcessor {
	public:
		static BasicProcessor& getInstance() {
			static BasicProcessor theProc;
			return theProc;
		}

		UniqueView process(UniqueStatement& aStatement) override {
			BasicView theView = ViewGenerator::generate_basic(view, aStatement);
			return std::make_unique<BasicView>(theView);
		}
	protected:
		// Contain a view, creating a view constantly might be tedious
		BasicView view;
		//AppProcessor &appProc = DBProcessor::getInstance();
	};
}

#endif // BasicProcessor.hpp