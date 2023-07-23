//
//  ErrorView.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef ErrorView_hpp
#define ErrorView_hpp

#include "View.hpp"
#include "../../Misc/Types/Errors.hpp"

namespace ECE141 {
	class ErrorView : public View {
	public:
		OptString createView(StatusResult& aStatus) {
			view = theErrors.count(aStatus.error) ? theErrors[aStatus.error] : "Unknown Error";
			return view;
		}

		OptString getView() const {return view;}

		bool    show(std::ostream& aStream) {
			aStream << view.value();
			return true;
		}
	protected:
		OptString view;

	private:
		std::map<Errors, std::string_view> theErrors = {
			{Errors::illegalIdentifier, "Error 12: Illegal identifier"},
			{Errors::unknownIdentifier, "Error 2: Unknown identifier"},
			{Errors::databaseExists, "Error 3: Database exists"},
			{Errors::tableExists, "Error 4: Table Exists"},
			{Errors::syntaxError, "Error 5: Syntax Error"},
			{Errors::unknownCommand, "Error 6: Unknown command"},
			{Errors::unknownDatabase,"Error 7: Unknown database"},
			{Errors::unknownTable,   "Error 8: Unknown table"},
			{Errors::unknownError,   "Error 9: Unknown error"},
			{Errors::invalidAttribute, "Error 10: Invalid Attribute"},
			{Errors::unknownType, "Error 11: Unknown Type"},
			{Errors::unexpectedValue, "Error 12: Illegal length value"},
			{Errors::invalidCommand, "Error 13: Invalid command"},
		};
		
	};

}

#endif /* ErrorView_hpp */