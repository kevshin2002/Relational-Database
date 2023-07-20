//
//  ViewGenerator.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
// 
//

#ifndef ViewGenerator_hpp
#define ViewGenerator_hpp

//#include "DBView.hpp"
//#include "TableView.hpp"
//#include "StringView.hpp"
#include "Views/ErrorView.hpp" 

namespace ECE141 {
	struct ViewGenerator {
		//static DBView& DB_View(ICommand& aCommand, DatabaseUnique& aDB, DatabaseProcessor& aProcessor) {
			//static DBView dbView;
			//dbView.createView(aCommand, aDB, aProcessor);
			//return dbView;
		//}
		//static TableView& Table_View(ICommand& aCommand, Table& aTable, TableProcessor& aProcessor) {
			//static TableView tableView;
			//tableView.createView(aCommand, aTable, aProcessor);
			//return tableView;
		//}
		//static StringView& String_View() { // Is this necessary to complete for
		//	static StringView stringView;
		//	stringView.createView(aCommand);
		//	return stringView;
		//}
		static ErrorView& generate_error(ErrorView& aView, StatusResult& aStatus) {
			aView.createView(aStatus);
			return aView;
		}
	};

}
#endif /* ViewGenerator_hpp */