//
//  Attribute.hpp
//  PA3
//
//  Created by rick gessner on 4/18/22.
//  Copyright © 2023 rick gessner. All rights reserved.
//

#include "Attribute.hpp"

namespace ECE141 {
  Attribute::Attribute(DataTypes aType) : type(DataTypes::no_type), size(0), primary(false), increment(false), nullable(false), unique(false) {}
  Attribute::Attribute(std::string aName, DataTypes aType, uint32_t aSize)  {
    name=aName;
    type = aType;
    size = aSize;
  }
 
  Attribute::Attribute(const Attribute &aCopy)  {
      name = aCopy.name;
      type = aCopy.type;
      size = aCopy.size;
      primary = aCopy.primary;
      increment = aCopy.increment;
      nullable = aCopy.nullable;
      unique = aCopy.unique;
  }
 
  Attribute::~Attribute()  {
  }
  
  bool Attribute::isValid() { // check if types match
    return true;
  }

  Attribute& Attribute::setName(const std::string& aString) {
      name = aString;
      return *this;
  }

  Attribute& Attribute::setSize(const size_t& aSize) {
      size = aSize;
      return *this;
  }

  Attribute& Attribute::setDataType(const DataTypes& aType) {
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
  Attribute& Attribute::setUnique(bool aBool) {
      unique = aBool;
      return *this;
  }

}
