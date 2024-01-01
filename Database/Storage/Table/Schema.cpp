//
//  Schema.cpp
//  PA3
//
//  Created by rick gessner on 3/2/23.
//

#include "Schema.hpp"

namespace ECE141 {

    //STUDENT: Implement this class...
  Schema::Schema(const std::string& aName) : name(aName), hashedName(Helpers::hashString(aName)) {}
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

  std::istream& Schema::operator<<(std::istream& anInput) {
      while (anInput) {
          Attribute theAttribute;
          std::string theValue;

          anInput >> theValue;
          theAttribute.setName(theValue);

          anInput >> theValue;
          theAttribute.setDataType(Helpers::stringToDataType(theValue));
          if (theAttribute.getType() == DataTypes::varchar_type) {
              anInput >> theValue;
              theAttribute.setSize(std::stoi(theValue));
          }
          anInput >> theValue;
          while (anInput) {
              setAttribute(theAttribute, theValue);
              if (theValue[0] == '\\')
                  break;
              anInput >> theValue;
          }
          if(theAttribute.getType() != DataTypes::no_type)
            attributes.push_back(theAttribute);
      }

      return anInput;
  }
  std::ostream& Schema::operator>>(std::ostream& anOutput) {
      for (const auto& theAttribute : attributes) {
          anOutput << theAttribute.getName() << " ";

          if (theAttribute.getType() == DataTypes::varchar_type)
              anOutput << Helpers::dataTypeToString(theAttribute.getType()) << " " << theAttribute.getSize() << " ";
          else
              anOutput << Helpers::dataTypeToString(theAttribute.getType()) << " ";

          anOutput << "primary:" << theAttribute.isPrimary() << " "
              << "increment:" << theAttribute.isIncrement() << " "
              << "nullable:" << theAttribute.isNull() << " "
              << "unique:" << theAttribute.isUnique() << " \\ ";
      }
      return anOutput;
  }
  BlockHeader Schema::initHeader() {return BlockHeader(BlockType::schema_block, hashedName); }

  Schema& Schema::addAttribute(const Attribute& anAttribute) {
      attributes.push_back(anAttribute);
      return *this;
  }

  Schema& Schema::setName(const std::string& aName) {
      name = aName;
      hashedName = Helpers::hashString(aName);
      return *this;
  }

  Schema& Schema::setAttribute(Attribute& aAttribute, const std::string& aString) {
      // fix *(aString.end() - 1) == '1' ? true : false
      switch (Helpers::charToExtra(aString[0])) {
      case Keywords::primary_kw:
          aAttribute.setPrimaryKey(*(aString.end() - 1) == '1' ? true : false);
          break;
      case Keywords::auto_increment_kw:
          aAttribute.setAutoIncrement(*(aString.end() - 1) == '1' ? true : false);
          break;
      case Keywords::null_kw:
          aAttribute.setNullable(*(aString.end() - 1) == '1' ? true : false);
          break;
      case Keywords::unique_kw:
          aAttribute.setUnique(*(aString.end() - 1) == '1' ? true : false);
          break;
      }
      return *this;
  }
  Attribute* Schema::getAttribute(const std::string& aField) {
      auto theAttribute = (std::find_if(attributes.begin(), attributes.end(), [&](const Attribute& anAttribute) {
              return anAttribute.getName() == aField;
          }));

      return theAttribute != attributes.end()  ? &(*theAttribute) : nullptr;
  }
 
  

}
