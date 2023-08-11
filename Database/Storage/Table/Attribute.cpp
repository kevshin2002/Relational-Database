//
//  Attribute.hpp
//  PA3
//
//  Created by rick gessner on 4/18/22.
//  Copyright © 2023 rick gessner. All rights reserved.
//

#include "Attribute.hpp"

namespace ECE141 {
  Attribute::Attribute(DataTypes aType) : type(DataTypes::no_type), size(0), primary(false), increment(false), nullable(false) {}
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

  Attribute& Attribute::setSize(size_t aSize) {
      size = aSize;
      return *this;
  }

  Attribute& Attribute::setDataType(DataTypes aType) {
      type = aType;
      return *this;
  }
  Attribute& Attribute::setPrimaryKey(bool aBool) {
      primary = aBool;
      return *this;
  }
  Attribute& Attribute::setAutoIncrement(bool aBool) {
      increment = aBool;
      return *this;
  }
  Attribute& Attribute::setNullable(bool aBool) {
      nullable = aBool;
      return *this;
  }


}
