//
//  Schema.hpp
//  PA3
//
//  Created by rick gessner on 3/18/23.
//  Copyright © 2023 rick gessner. All rights reserved.
//

#ifndef Schema_hpp
#define Schema_hpp

#include "Row.hpp"
#include "../../../Misc/Types/Errors.hpp"


namespace ECE141 {
  class Database;
  class Schema {
  public:
                          Schema(Database* aDB);
                          Schema(const Schema &aCopy);
    
                          ~Schema();
    Schema&               setName(const std::string& aName); 

    size_t                hashString(const std::string& aField);
    const std::string&    getName() const {return name;}
    Attribute*            getAttribute(const std::string& aField);
    

    size_t         getHash() const { return hashedName; }
  protected:
    Database*     database;
    AttributeList   attributes;
    size_t          hashedName;
    std::string     name;
    
    //how will you manage creation of primary keys?
    
  };
  
}
#endif /* Schema_hpp */
