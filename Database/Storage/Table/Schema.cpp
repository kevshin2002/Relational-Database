//
//  Schema.cpp
//  PA3
//
//  Created by rick gessner on 3/2/23.
//

#include "Schema.hpp"

namespace ECE141 {

 //STUDENT: Implement this class...

  Schema::Schema(Database* aDB) : database(aDB) {}

  Schema::Schema(const Schema &aCopy) : database(aCopy.database) {
    name=aCopy.name;
    attributes = aCopy.attributes;
    hashedName = aCopy.hashedName;
  }

  Schema::~Schema() {
      database = nullptr;
    //std::cout << "~Schema()\n";
  }

  Schema& Schema::addAttribute(const Attribute& anAttribute) {
      attributes.push_back(anAttribute);
      return *this;
  }

  Schema& Schema::setName(const std::string& aName) {
      name = aName;
      hashString(aName);
      return *this;
  }
  Attribute* Schema::getAttribute(const std::string& aField) {
      auto theAttribute = (std::find_if(attributes.begin(), attributes.end(), [&](const Attribute& anAttribute) {
              return anAttribute.getName() == aField;
          }));

      return theAttribute != attributes.end()  ? &(*theAttribute) : nullptr;
  }
 
  uint32_t  Schema::hashString(const std::string& aField) {
      hashedName = 0;
      for (char theChar : aField) {
          hashedName = hashedName * 31 + static_cast<uint32_t>(theChar);
      }
      return hashedName;
  }

}
