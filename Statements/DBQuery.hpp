//
//  DBQuery.hpp
//  PA5
//
//  Created by rick gessner on 4/7/23.
//

#ifndef DBQuery_hpp
#define DBQuery_hpp

#include "../Database/Storage/Table/Schema.hpp"
#include "../Utilities/Filters.hpp"

namespace ECE141 {

  // State held from a fancy select statement

  struct Property {
    Property(std::string aName, uint32_t aTableId=0) : name(aName), tableId(aTableId), desc(true) {}
    std::string     name;
    uint32_t        tableId;
    bool            desc;
  };

  using PropertyList = std::vector<Property>;

  //--------------------------
  class DBQuery {
  public:

    DBQuery(Schema *aSchema=nullptr, bool allFields=true) 
      : fromTable(aSchema), all(allFields) {}
    
    DBQuery(const DBQuery &aQuery) : fromTable(aQuery.fromTable) {}
                 
    //from (table) -- *, or a comma separated list of fields
    //where specific options...
    //offset
    //limit
    //group, etc.
    //maybe joins?  
    DBQuery& addAttribute(const Attribute& anAttribute) {
        attributes.push_back(anAttribute);
        return *this;
    }

    DBQuery& addIdentifiers(const StringList& aList) {
        identifiers = aList;
        return *this;
    }

    DBQuery& addValues(const StringList& aList) {
        values = aList;
        return *this;
    }

 //   bool Matches(const ) const {
   //   return true;
    //}


    const AttributeList& getAttributes() const { return attributes; }
    const StringList& getIdentifiers() const { return identifiers; }
    const StringList& getValues() const { return values; }
        
  protected:
    Schema*  fromTable;

    AttributeList attributes;
    StringList identifiers;
    StringList values;

    Filters  filters;
    bool     all; //if user used SELECT * FROM...

    //e.g. what if the user says, "SELECT name, age FROM students WHERE..."

  };

}

#endif /* DBQuery_h */


