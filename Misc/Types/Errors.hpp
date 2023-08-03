//
//  basetypes.hpp
//  Database
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#ifndef Errors_h
#define Errors_h

#include "BasicTypes.hpp"

namespace ECE141 {
    

  enum class Errors : uint16_t {
    
    noError=0,

    //parse related...
    keywordExpected=100,
    identifierExpected,
    illegalIdentifier,
    unknownIdentifier,
    unexpectedIdentifier,
    joinTypeExpected,
    keyExpected,
    syntaxError,
    unexpectedKeyword,
    unexpectedValue,
    valueExpected,
    operatorExpected,
    punctuationExpected,
    eofError,
    
    //statement related...
    statementExpected=200,
    noDatabaseSpecified,
    
    //table/DB related...
    tableExists=300,
    unknownEntity,
    unknownTable,
    unknownDatabase,
    databaseExists,
    databaseDeletionError,
    databaseCreationError,
    primaryKeyRequired,
    
    //type related...
    unknownType=400,
    unknownAttribute,
    invalidAttribute,
    invalidArguments,
    keyValueMismatch, //# of fieldnames doesn't match values...
    
    //storage/io related...
    readError=500,
    writeError,
    seekError,
    storageFull,
    
    //index related
    indexExists=600,
    cantCreateIndex,
    unknownIndex,
    
    //command related...
    unknownCommand=3000,
    noSemicolon,
    invalidCommand,
    
    //general purpose...
    userTerminated = 4000,
    notImplemented,
    
    unknownError = 10000
  };    
  
  struct StatusResult {
    Errors    error;
    uint32_t  value;
    
    StatusResult(Errors anError=Errors::noError) :
      error(anError), value(0) {}
    
    operator bool() {return Errors::noError==error;}
    bool operator==(Errors anError) {return anError==error;}
  };
 
}

#endif /* errors */
