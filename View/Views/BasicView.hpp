//
//  BasicView.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef BasicView_hpp
#define BasicView_hpp

#include "View.hpp"
#include "../../Misc/Types/Errors.hpp"

namespace ECE141 {
	class BasicView : public View {
	public:
		OptString createView(Statement& aStatement) {
			view = theBasics.count(aStatement.getType()) ? theBasics[aStatement.getType()] : "Unknown Command";
			return view;
		}

		OptString getView() const { return view; }

		bool    show(std::ostream& aStream) {
			aStream << view.value();
			return true;
		}
	protected:
		OptString view;

	private:
		std::map<StatementType, std::string_view> theBasics = {
			{StatementType::about, "Authors: Kevin Shin"},
			{StatementType::version, "Version: 0.1"},
			{StatementType::help, "Help incoming soon..."},
			{StatementType::quit, "DB::141 is shutting down"}
		};

	};

}

#endif /* BasicView_hpp */