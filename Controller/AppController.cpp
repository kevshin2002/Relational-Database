//
//  CommandProcessor.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#include "AppController.hpp"


namespace ECE141 {
  
  AppController::AppController() : running{true} {}
  
  AppController::~AppController() {}
  
  // USE: -----------------------------------------------------
  
  //build a tokenizer, tokenize input, ask processors to handle...
  StatusResult AppController::handleInput(std::istream &anInput,
                                        ViewListener aViewer){
    Tokenizer theTokenizer(anInput);
    StatusResult theResult=theTokenizer.tokenize();
    CommandFactory cmdFac(theTokenizer);
    if (hasSemicolon(theTokenizer, theResult)) {
        cmdFac.factorize(theResult);
        FactoryRouter facRouter;
        while (theResult && cmdFac.more()) {
            UniqueStatement Statement = facRouter.route(theResult, cmdFac.getCmd());
            if (theResult || theResult.error == Errors::userTerminated) {
                UniqueView theView = appProc.process(Statement);
                aViewer(*theView);
            }
            cmdFac.deleteCmds(Quantity::one);
        }
    }

    if (theResult.error != Errors::noSemicolon && theResult.error != Errors::userTerminated) {
        emptyCache();
        cmdFac.deleteCmds(Quantity::all);
    }
    return theResult;
    /*
    if tokenize.validate() == true
    command factory (tokenizer) -> creates different commands
    COR recognize pure virtual interface - make it into statements
    statements - execute

    
    */
    
    return theResult;
  }
  StatusResult AppController::hasSemicolon(Tokenizer& aTokenizer, StatusResult& aResult) {
      Token theLastToken = aTokenizer.getTokens().back();
      aResult = Errors::noSemicolon;
      cache.insert(cache.end(), aTokenizer.getTokens().begin(), aTokenizer.getTokens().end());
      if (static_cast<size_t>(Quantity::one) == std::count(theLastToken.data.begin(), theLastToken.data.end(), ';')) {
          aResult = Errors::noError;
          cache.pop_back();
          aTokenizer.getTokens() = cache;
      }
      return aResult;
  }
  OptString AppController::getError(StatusResult &aResult) {
      OptString optString = std::nullopt;
      if (aResult.error != Errors::noSemicolon)
          optString = errorProc.getView(aResult);
      return optString;
  }




}
