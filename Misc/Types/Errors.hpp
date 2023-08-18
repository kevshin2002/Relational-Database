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
        // Parse related
        creationOpenerExpected = 100,
        creationCloserExpected,
        insertIdentifiersOpenerExpected,
        insertIdentifiersCloserExpected,
        insertValuesOpenerExpected,
        insertValuesCloserExpected,
        illegalAttributeIdentifier,
        illegalIdentifier,
        identifierExpected,
        joinTypeExpected,
        keywordExpected,
        keyExpected,
        keyValueMismatch,

        // General errors
        noError = 0,

        // Statement related
        noDatabaseSpecified = 200,
        primaryKeyRequired,
        syntaxError,
        insertValuesError,
        
        unexpectedKeyword,
        unexpectedValue,
        unknownIdentifier,
        unexpectedIdentifier,
        valueExpected,
        operatorExpected,
        punctuationExpected,
        eofError,

        // Statement related
        statementExpected = 300,
        noSemicolon,
        invalidCommand,

        // Table/DB related
        tableExists,
        invalidTableName,
        unknownEntity,
        unknownTable,
        unknownDatabase,
        databaseExists,
        databaseDeletionError,
        databaseCreationError,

        // Type related
        unknownDatatype = 400,
        unknownType,
        unknownAttribute,
        invalidAttribute,
        invalidArguments,

        // Storage/IO related
        readError = 500,
        writeError,
        seekError,
        storageFull,

        // Index related
        cantCreateIndex = 600,
        unknownIndex,

        // Command related
        notImplemented = 4000,
        userTerminated,
        unknownCommand = 3000,

        // General purpose
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
