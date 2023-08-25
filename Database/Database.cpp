//
//  Database.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//


#include "Database.hpp"


namespace ECE141 {
  
  Database::Database(const std::string aName, AccessMode aMode)
    : storage(Config::getDBPath(aName), aMode), name(aName) {
    //create for read/write
  }

  Database::~Database() {
  }

  StatusResult Database::dump(std::ostream& anOutput) {
      return StatusResult{ Errors::noError };
  }

  StatusResult    Database::fetchTables(std::set<std::string>& aTableList) {
      return inUse() ? storage.fetchTables(aTableList) : Errors::noDatabaseSpecified;
  }

  bool Database::inUse(const std::string& aDBName) {
      bool theResult = false;
      if (aDBName.size() && this) {
          theResult = aDBName == name ? true : theResult;
      }
      else
          theResult = this ? true : theResult;
      return theResult;
  }

  Schema* Database::getSchema(const std::string& aName) {
      return storage.getSchema(aName);
  }

  TableOpt   Database::getTable(const std::string& aName) {
      TableOpt theTable = *storage.getTable(aName);
      return theTable;
  }

  // USE: Dump command for debug purposes...

}
