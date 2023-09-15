//
//  Row.cpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//


#include "Row.hpp"

namespace ECE141 {
  
  Row::Row(uint32_t aID) : rowID(aID) {}
  Row::Row(const Row &aRow) {*this=aRow;}
  Row::Row(Row&& aSource) : data(std::move(aSource.data)) {}
  Row::~Row() {}

  Row& Row::operator=(const Row &aRow) {
      rowID = aRow.rowID;
      data = aRow.data;
      return *this;}
  bool Row::operator==(const Row& aCopy) const {
      return rowID == aCopy.rowID ? true : false;
  }

  //STUDENT: What other methods do you require?
                      
  Row& Row::set(const std::string &aKey,const Value &aValue) {
    return *this;
  }

  // RowCollection
  //-------------------------------------------------------
  RowCollection::RowCollection(uint32_t aSchemaID) {
      schemaID = aSchemaID;
  }
  RowCollection::RowCollection(const RowCollection& aCollection) {
      *this = aCollection;
  }
  RowCollection::~RowCollection() {}
  RowCollection& RowCollection::operator=(const RowCollection& aCollection) {
      rows = aCollection.rows;
      schemaID = aCollection.schemaID;
      return *this;
  }
  std::istream& RowCollection::operator<<(std::istream& anInput) { return anInput; }
  std::ostream& RowCollection::operator>>(std::ostream& anOutput) {
      for (auto& theRow : rows) {
          auto& theDatas = theRow.getData();
          for (const auto& theData : theDatas) {
              anOutput << theData.first << ":" << std::get<std::string>(theData.second) << " \\ ";
          }
      }
      return anOutput;
  }
  BlockHeader RowCollection::initHeader() { return BlockHeader(BlockType::data_block, schemaID); }
  bool RowCollection::add(Row& aRow) {
      rows.push_back(aRow);
      return true;
  }

  bool RowCollection::remove(Row& aRow) {
      bool theResult = false;
      auto theRow = std::find(rows.begin(), rows.end(), aRow);
      if (theRow != rows.end()) {
        rows.erase(theRow);
         theResult = true;
    }
      return theResult;     
  }
}
