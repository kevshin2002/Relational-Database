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
  //Row::Row(Row&& aSource) : data(std::move(aSource.data)) {}
  Row::~Row() {}

  Row& Row::operator=(const Row &aRow) {
      rowID = aRow.rowID;
      data = aRow.data;
      return *this;}
  bool Row::operator==(Row &aCopy) const {return false;}

  //STUDENT: What other methods do you require?
                      
  Row& Row::set(const std::string &aKey,const Value &aValue) {
    return *this;
  }
    
}
