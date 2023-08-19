//
//  Database.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//


#include "Database.hpp"


namespace ECE141 {
  
  Database::Database(const std::string aName, AccessMode aMode)
    : name(aName), storage(Config::getDBPath(aName), aMode), changed(true) {
    //create for read/write
  }


  Database::~Database() {
    if(changed) {
      //stuff to save?
    }
  }

  StatusResult    Database::fetchTables(std::set<std::string> aTableList) {
      return Errors::notImplemented;
  }

  TableOpt        Database::getTable(Schema& aSchema) {
      return std::nullopt;
  }
  bool Database::inUse(const std::string& aDBName) const {
      bool theResult = false;
      if (aDBName.size() && this) { 
          theResult = aDBName == name ? true : theResult;
      }
      else
          theResult = this ? true : theResult;
      return theResult;
  }
  // USE: Dump command for debug purposes...
  StatusResult Database::dump(std::ostream &anOutput) {    
    return StatusResult{Errors::noError};
  }

}
