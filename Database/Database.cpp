//
//  Database.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include "Database.hpp"
#include "../Utilities/Config.hpp"

namespace ECE141 {
  
  Database::Database(const std::string aName, AccessMode aMode)
    : name(aName), changed(true) {
    std::string thePath = Config::getDBPath(name);
    //create for read/write
  }


  Database::~Database() {
    if(changed) {
      //stuff to save?
    }
  }

  // USE: Dump command for debug purposes...
  StatusResult Database::dump(std::ostream &anOutput) {    
    return StatusResult{Errors::noError};
  }

}
