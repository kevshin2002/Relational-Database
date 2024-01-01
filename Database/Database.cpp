//
//  Database.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//


#include "Database.hpp"


namespace ECE141 {
  
  Database::Database(const std::string aName, AccessMode aMode)
    : storage(Config::getDBPath(aName), aMode), name(aName) {
    //create for read/write
  }

  Database::~Database() {
  }

  StatusResult Database::dump(std::ostream& anOutput) {
      auto& theIndices = storage.getIndices();
      size_t theExtraLength = theIndices.size() ? std::max_element(theIndices.begin(), theIndices.end(),
          [](const auto& aPair_One, const auto& aPair_Two) {
              return aPair_One.first.second.size() < aPair_Two.first.second.size();
          })->first.second.size() + 10 : 10;
     
    anOutput << "+" << std::setfill('-') << std::setw(16);
    anOutput << "+" << std::setfill('-') << std::setw(14);
    anOutput << "+" << std::setfill('-') << std::setw(theExtraLength) << "+\n";

    anOutput << "| Type " << std::setfill(' ') << std::setw(16 - 6) << "|";
    anOutput << " Position " << std::setfill(' ') << std::setw(8 - 4) << "|";
    anOutput << " Extra " << std::setfill(' ') << std::setw(theExtraLength - 7) << "|\n";
    
    anOutput << "+" << std::setfill('-') << std::setw(16);
    anOutput << "+" << std::setfill('-') << std::setw(14);
    anOutput << "+" << std::setfill('-') << std::setw(theExtraLength) << "+\n";

    for (const auto& aBlock : theIndices) {
        std::string theValue = Helpers::blockTypeToString(aBlock.second);
        anOutput << "| " << theValue << std::setfill(' ') << std::setw(16  - theValue.size());
        anOutput << "| " << aBlock.first.first << std::setfill(' ') << std::setw(14 - std::to_string(aBlock.first.first).size());
        anOutput << "| " << aBlock.first.second << std::setfill(' ') << std::setw(theExtraLength - aBlock.first.second.size() - 1) << "|\n";
    }
    anOutput << "+" << std::setfill('-') << std::setw(16);
    anOutput << "+" << std::setfill('-') << std::setw(14);
    anOutput << "+" << std::setfill('-') << std::setw(theExtraLength) << "+\n";
    anOutput << theIndices.size() << " rows in set";
    return Errors::noError;
  }

  StatusResult    Database::fetchTables(std::set<std::string>& aTableList) {
      return inUse() ? storage.fetchTables(aTableList) : Errors::noDatabaseSpecified;
  }

  bool Database::inUse(const std::string& aDBName) {
      bool theResult = false;
      if (aDBName.size() && this) {
          theResult = aDBName == name ? true : theResult;
      }
      else
          theResult = this ? true : theResult;
      return theResult;
  }

  Schema* Database::getSchema(const std::string& aName) {
      return storage.getSchema(aName);
  }


  // USE: Dump command for debug purposes...

}
