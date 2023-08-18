//
//  AppController.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#include "AppController.hpp"


namespace ECE141 {
  
  AppController::AppController() : running{ true }, db(nullptr), next(new DBProcessor()) {}
  AppController::~AppController() { 
      delete next; 
      delete db;
  }
  // USE: -----------------------------------------------------
  
  //build a tokenizer, tokenize input, ask processors to handle...
  StatusResult AppController::handleInput(std::istream& anInput,
                                        ViewListener aViewer){
    Config::getTimer().reset();
    Tokenizer theTokenizer(anInput);
    StatusResult theResult=theTokenizer.tokenize(terminator);
    
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
              "create database {name} : make a new database in storage folder\n"\
              "drop database {name}   : delete a known database from storage folder\n"\
              "show databases         : list databases in storage folder\n"\
              "use database {name}    : load a known database for use\n"\
              "about                  : show members\n"\
              "help                   : show list of commands\n"\
              "quit                   : stop app\n"\
              "version                : show app version";
          break;
      case StatementType::quit:
          theView = "DB::141 is shutting down";
          running = false;
          break;
      }
      aListener(theView);
      return Errors::noError;
  }
  
  bool AppController::holdDB(Database* aDB) {
      db = aDB;
      return true;
  }
  
  bool AppController::releaseDB() {
      delete db;
      db = nullptr;
      return true;
  }

  OptString AppController::getError(StatusResult& aResult) const {

      static std::map<Errors, std::string_view> theMessages = {
      {Errors::illegalAttributeIdentifier, "Illegal attribute identifier"},
      {Errors::illegalIdentifier, "Illegal identifier"},
      {Errors::identifierExpected, "Identifier expected"},
      {Errors::joinTypeExpected, "Join type expected"},
      {Errors::keywordExpected, "Keyword expected"},
      {Errors::keyExpected, "Key expected"},
      {Errors::keyValueMismatch, "Key-value mismatch"},
      {Errors::noError, "No error"},
      {Errors::noDatabaseSpecified, "No database specified"},
      {Errors::primaryKeyRequired, "Primary key required"},
      {Errors::syntaxError, "Syntax error"},
      {Errors::insertValuesError, "Insert values error"},
      {Errors::creationOpenerExpected, "Column definition '(' expected"},
      {Errors::creationCloserExpected, "Column definition ')' expected"},
      {Errors::insertIdentifiersOpenerExpected, "Identifier list '(' expected"},
      {Errors::insertIdentifiersCloserExpected, "Identifier list ')' expected"},
      {Errors::insertValuesOpenerExpected, "Values list '(' expected"},
      {Errors::insertValuesCloserExpected, "Values list ')' expected"},
      {Errors::unexpectedKeyword, "Unexpected keyword"},
      {Errors::unexpectedValue, "Unexpected value"},
      {Errors::unknownIdentifier, "Unknown identifier"},
      {Errors::unexpectedIdentifier, "Unexpected identifier"},
      {Errors::valueExpected, "Value expected"},
      {Errors::operatorExpected, "Operator expected"},
      {Errors::punctuationExpected, "Punctuation expected"},
      {Errors::eofError, "End of file error"},
      {Errors::statementExpected, "Statement expected"},
      {Errors::noSemicolon, "No semicolon"},
      {Errors::invalidCommand, "Invalid command"},
      {Errors::tableExists, "Table exists"},
      {Errors::invalidTableName, "Invalid table name"},
      {Errors::unknownEntity, "Unknown entity"},
      {Errors::unknownTable, "Unknown table"},
      {Errors::unknownDatabase, "Unknown database"},
      {Errors::databaseExists, "Database exists"},
      {Errors::databaseDeletionError, "Database deletion error"},
      {Errors::databaseCreationError, "Database creation error"},
      {Errors::unknownDatatype, "Unknown Datatype"},
      {Errors::unknownType, "Unknown type"},
      {Errors::unknownAttribute, "Unknown attribute"},
      {Errors::invalidAttribute, "Invalid attribute"},
      {Errors::invalidArguments, "Invalid arguments"},
      {Errors::readError, "Read error"},
      {Errors::writeError, "Write error"},
      {Errors::seekError, "Seek error"},
      {Errors::storageFull, "Storage full"},
      {Errors::cantCreateIndex, "Cannot create index"},
      {Errors::unknownIndex, "Unknown index"},
      {Errors::notImplemented, "Not implemented"},
      {Errors::userTerminated, "User terminated"},
      {Errors::unknownCommand, "Unknown command"},
      {Errors::unknownError, "Unknown error"}
      };

      std::string_view theMessage = "Unknown Error";
      if (theMessages.count(aResult.error)) {
          theMessage = theMessages[aResult.error];
      }
      return theMessage;
  }




}
