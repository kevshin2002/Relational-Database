//
//  AppController.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#include "AppController.hpp"


namespace ECE141 {
  
  AppController::AppController() : running{true} {
      db = nullptr;
      next = new DBProcessor();
  }
  
  AppController::~AppController() { 
      delete next; 
      if (db) // safety
          delete db;
  }
  // USE: -----------------------------------------------------
  
  //build a tokenizer, tokenize input, ask processors to handle...
  StatusResult AppController::handleInput(std::istream& anInput,
                                        ViewListener aViewer){
    Tokenizer theTokenizer(anInput);
    StatusResult theResult=theTokenizer.tokenize();
    
    while (theResult && theTokenizer.more(1)) {
        theResult = Errors::unknownCommand;
        if (auto* theProc = findHandler(theTokenizer)) {
            if (auto* theStmt = theProc->makeStatement(theTokenizer, this)) {
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
    return theResult;
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

  AppProcessor* AppController::findHandler(Tokenizer& aTokenizer) {
      return isProcessable(aTokenizer.current().keyword) ? this : next->findHandler(aTokenizer);
  }

  Statement* AppController::makeStatement(Tokenizer& aTokenizer, AppController* anAppController) {
      return new Statement(StatementType::unknown);
  }

  StatusResult AppController::run(Statement* aStatement, ViewListener aListener) {
      StringView theView;
      switch (aStatement->getType()) {
      case StatementType::about:
          theView = "Authors: Kevin Shin";
          break;
      case StatementType::version:
          theView = "Version: 0.1";
          break;
      case StatementType::help:
          theView =
              "create database {name}: make a new database in storage folder\n"\
              "drop database{name}   : delete a known database from storage folder\n"\
              "show databases        : list databases in storage folder\n"\
              "use database{ name }  : load a known database for use\n"\
              "about                 : show members\n"\
              "help                  : show list of commands\n"\
              "quit                  : stop app\n"\
              "version               : show app version";
          break;
      case StatementType::quit:
          theView = "DB::141 is shutting down";
          running = false;
          break;
      default: // Is this necessary? isProcessably already validates. Adding for sake of completion
          return Errors::invalidCommand;
      }
      aListener(theView);
      return Errors::noError;
  }
  
  bool AppController::holdDB(Database* aDB) {
      db = aDB;
      return true;
  }
  
  bool AppController::releaseDB() {
      if (db)
          delete db;
      return true;
  }

  OptString AppController::getError(StatusResult& aResult) const {

      static std::map<ECE141::Errors, std::string_view> theMessages = {
        {Errors::illegalIdentifier, "Illegal identifier"},
        {Errors::identifierExpected, "Identifier expected"},
        {Errors::unknownIdentifier, "Unknown identifier"},
        {Errors::databaseExists, "Database exists"},
        {Errors::tableExists, "Table Exists"},
        {Errors::syntaxError, "Syntax Error"},
        {Errors::unknownCommand, "Unknown command"},
        {Errors::unknownDatabase,"Unknown database"},
        {Errors::unknownTable,   "Unknown table"},
        {Errors::unknownError,   "Unknown error"},
        {Errors::notImplemented, "Not Implemented"}
      };

      std::string_view theMessage = "Unknown Error";
      if (theMessages.count(aResult.error)) {
          theMessage = theMessages[aResult.error];
      }
      return theMessage;
  }




}
