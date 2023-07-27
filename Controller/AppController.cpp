//
//  CommandProcessor.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#include "AppController.hpp"


namespace ECE141 {
  
  AppController::AppController() : running{true} {
      db = std::unique_ptr<Database>();
      next = 
  }
  
  AppController::~AppController() {}
  
  // USE: -----------------------------------------------------
  
  //build a tokenizer, tokenize input, ask processors to handle...
  StatusResult AppController::handleInput(std::istream& anInput,
                                        ViewListener aViewer){
    Tokenizer theTokenizer(anInput);
    StatusResult theResult=theTokenizer.tokenize();
    
    while (theResult && theTokenizer.more()) {
        theResult = Errors::unknownCommand;
        if (auto *theProc = findHandler(theTokenizer)) {
            if (auto *theStmt = theProc->makeStatement(theTokenizer, this)) {
                theResult = theStmt->parse(theTokenizer);
                if (theResult) {
                    theResult = theProc->run(theStmt, aViewer);
                    delete theStmt;
                }
            }
        }

        if (!theResult) {
            std::string theMsg(getError(theResult).value());
            if (theResult.value) {
                theMsg += ":" + theTokenizer.tokenAt(theResult.value).data;
            }
            StringView theView(theMsg);
            aViewer(theView);
        }
    }
  }

  bool AppController::isProcessable(Keywords& aKeyword) const {
      switch (aKeyword) {
      case Keywords::about_kw:
      case Keywords::version_kw:
      case Keywords::help_kw:
      case Keywords::quit_kw:
          return true;
      default:
          return false;
      }
  }

  AppController* AppController::findHandler(Tokenizer& aTokenizer) {
      return isProcessable(aTokenizer.current().keyword) ? this : next->findHandler(aTokenizer);
  }

  StatusResult AppController::run(Statement* aStatement, ViewListener aListener) {

  }

  Statement* AppController::makeStatement(Tokenizer& aTokenizer, AppController* anAppController) {

  }
  
  OptString AppController::getError(StatusResult& aResult) const {

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

      std::string_view theMessage = "Unknown Error";
      if (theMessages.count(aResult.error)) {
          theMessage = theMessages[aResult.error];
      }
      return theMessage;
  }




}
