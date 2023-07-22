//
//  ErrorProcessor.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//
#ifndef ErrorProcessor_hpp
#define ErrorProcessor_hpp

#include "../../View/ViewGenerator.hpp"

namespace ECE141 {
	class ErrorProcessor {
	public:
		std::string_view getView(StatusResult& aResult) {return ViewGenerator::generate_error(errorView, aResult).getView().value();}

	protected:
		ErrorView errorView;
	};
}



#endif // ErrorProcessor_hpp