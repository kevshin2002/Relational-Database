//
//  Schema.hpp
//  PA3
//
//  Created by rick gessner on 3/18/23.
//  Copyright © 2023 rick gessner. All rights reserved.
//

#ifndef Schema_hpp
#define Schema_hpp

#include <memory>
#include <string>

#include "Attribute.hpp"
#include "../../../Misc/Types/Errors.hpp"


namespace ECE141 {
  class Schema {
  public:
                          Schema(const std::string aName);
                          Schema(const Schema &aCopy);
    
                          ~Schema();
    
    const std::string&    getName() const {return name;}
    Attribute&            getAttribute(const std::string& aField);
           
  protected:
        
    AttributeList   attributes;
    std::string     name;
    
    //how will you manage creation of primary keys?
    
  };
  
}
#endif /* Schema_hpp */
