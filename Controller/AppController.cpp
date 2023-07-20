//
//  CommandProcessor.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#include "AppController.hpp"


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
    
    /*
    if tokenize.validate() == true
    command factory (tokenizer) -> creates different commands
    COR recognize pure virtual interface - make it into statements
    statements - execute

    
    */
    while (theResult && theTokenizer.more()) {      
      //how will we handle this input?
      theResult=Errors::unknownCommand;
    }
    return theResult;
  }

  OptString AppController::getError(StatusResult &aResult) {return errorProc.getView(aResult);}




}
