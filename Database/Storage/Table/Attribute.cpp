//
//  Attribute.hpp
//  PA3
//
//  Created by rick gessner on 4/18/22.
//  Copyright © 2023 rick gessner. All rights reserved.
//

#include "Attribute.hpp"

namespace ECE141 {

  Attribute::Attribute(DataTypes aType) {}
 
  Attribute::Attribute(std::string aName, DataTypes aType, uint32_t aSize)  {
    name=aName;
    //other fields?
  }
 
  Attribute::Attribute(const Attribute &aCopy)  {
    name=aCopy.name;
    //other fields?
  }
 
  Attribute::~Attribute()  {
  }
  
  bool Attribute::isValid() {
    return true;
  }


}
