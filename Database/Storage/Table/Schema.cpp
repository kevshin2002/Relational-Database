//
//  Schema.cpp
//  PA3
//
//  Created by rick gessner on 3/2/23.
//

#include "Schema.hpp"

namespace ECE141 {

 //STUDENT: Implement this class...

  Schema::Schema(const std::string aName) : name(aName){}

  Schema::Schema(const Schema &aCopy) {
    name=aCopy.name;
  }

  Schema::~Schema() {
    //std::cout << "~Schema()\n";
  }

  Attribute* Schema::getAttribute(const std::string& aField) {
      return &(*std::find_if(attributes.begin(), attributes.end(),
          [&](const Attribute& anAttribute) {
              return anAttribute.getName() == aField;
          }));
  }
 
  size_t  Schema::hashString(const std::string& aField) {
      std::hash<std::string> hashString;
      hashedName = hashString(aField);
      return hashedName;
  }

}
