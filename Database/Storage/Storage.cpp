//
//  Storage.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//


#include <sstream>
#include <cmath>
#include <cstdlib>
#include <optional>
#include <cstring>
#include <iostream>
#include "Storage.hpp"
#include "../../Utilities/Config.hpp"

namespace ECE141 {


  // USE: ctor ---------------------------------------

  Storage::Storage(const std::string &aName, AccessMode aMode)
    : BlockIO(aName, aMode) {}

  // USE: dtor ---------------------------------------
  Storage::~Storage() {
  }
  StatusResult Storage::add(BlockType aType, StatementType aStmtType, DBQuery* aQuery) { return Errors::notImplemented; }
  StatusResult Storage::drop(BlockType aType, StatementType aStmtType, DBQuery* aQuery) { return Errors::notImplemented; }
  bool Storage::each(const BlockVisitor &aVisitor) {
    return true;
  }

 
}

