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
 

}
