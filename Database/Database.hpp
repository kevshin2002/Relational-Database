//
//  Database.hpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include <fstream> 
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include "../Utilities/Config.hpp"
#include "Storage/Storage.hpp"

namespace ECE141 {

  //db should be related to storage somehow...

  class Database {
  public:    
            Database(const std::string aPath, AccessMode);            
    virtual ~Database();

    StatusResult    dump(std::ostream &anOutput); //debug...

    bool            inUse(const std::string& aDBName) const;
  protected:
    Storage         storage;
    std::string     name;
    bool            changed;  //might be helpful, or ignore if you prefer.
  };

}
#endif /* Database_hpp */
