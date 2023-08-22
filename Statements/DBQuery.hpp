//
//  DBQuery.hpp
//  PA5
//
//  Created by rick gessner on 4/7/23.
//

#ifndef DBQuery_hpp
#define DBQuery_hpp

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

    DBQuery(Database* aDB, bool allFields=true) 
      :  all(allFields) {
        fromTable = new Schema(aDB);
    }
    
    DBQuery(const DBQuery &aQuery) : fromTable(aQuery.fromTable) {}
    ~DBQuery() {}
    //from (table) -- *, or a comma separated list of fields
    //where specific options...
    //offset
    //limit
    //group, etc.
    //maybe joins?  

    DBQuery& addIdentifiers(const StringList& aList) {
        identifiers = aList;
        return *this;
    }

    DBQuery& addValues(const StringList& aList) {
        values = aList;
        return *this;
    }
    
    StatusResult setAll() { 
        all = true; 
        return Errors::noError; 
    }
 //   bool Matches(const ) const {
   //   return true;
    //}

    const StringList& getIdentifiers() const { return identifiers; }
    const StringList& getValues() const { return values; }
    Filters& getFilters() { return filters; }
    Schema* getSchema() { return fromTable; }
  protected:
    Schema*  fromTable;

    StringList identifiers;
    StringList values;

    Filters  filters;

    bool     all; //if user used SELECT * FROM...

    //e.g. what if the user says, "SELECT name, age FROM students WHERE..."

  };

}

#endif /* DBQuery_h */


