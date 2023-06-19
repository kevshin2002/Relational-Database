//
//  CommandProcessor.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#include <iostream>
#include <memory>
#include <vector>
#include "AppController.hpp"
#include "../Utilities/Tokenizer/Tokenizer.hpp"

namespace ECE141 {
  
  AppController::AppController(std::ostream &anOutput) : running{true} {
  }
  
  AppController::~AppController() {}
  
  // USE: -----------------------------------------------------
  
  //build a tokenizer, tokenize input, ask processors to handle...
  StatusResult AppController::handleInput(std::istream &anInput,
                                        ViewListener aViewer){
    Tokenizer theTokenizer(anInput);
    StatusResult theResult=theTokenizer.tokenize();
    
    while (theResult && theTokenizer.more()) {      
      //how will we handle this input?
      theResult=Errors::unknownCommand;
    }
    return theResult;
  }

  OptString AppController::getError(StatusResult &aResult) const {

    static std::map<ECE141::Errors, std::string_view> theMessages = {
      {Errors::illegalIdentifier, "Illegal identifier"},
      {Errors::unknownIdentifier, "Unknown identifier"},
      {Errors::databaseExists, "Database exists"},
      {Errors::tableExists, "Table Exists"},
      {Errors::syntaxError, "Syntax Error"},
      {Errors::unknownCommand, "Unknown command"},
      {Errors::unknownDatabase,"Unknown database"},
      {Errors::unknownTable,   "Unknown table"},
      {Errors::unknownError,   "Unknown error"}
    };

    std::string_view theMessage="Unknown Error";
    if(theMessages.count(aResult.error)) {
      theMessage=theMessages[aResult.error];
    }
    return theMessage;
  }




}
