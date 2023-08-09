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

  bool Database::inUse(const std::string& aDBName) const {
      return this ?
             aDBName == name ? true : false :
             false;
  }
  // USE: Dump command for debug purposes...
  StatusResult Database::dump(std::ostream &anOutput) {    
    return StatusResult{Errors::noError};
  }

}
