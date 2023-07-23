//
//  ScriptRunner.hpp
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef ScriptRunner_h
#define ScriptRunner_h

#include <fstream>
#include <sstream>
#include "../Controller/AppController.hpp"

namespace ECE141 {

  class ScriptRunner {
  public:
    ScriptRunner(AppController &anApp) : app(anApp) {}
      
    std::string readCommand(std::istream &anInput) {
      std::string theResult;
      char theChar;
      while(!anInput.eof()) {
        anInput >> theChar;
        if(anInput.eof() || 0==theChar) break;
        else theResult+=theChar;
      }
      return theResult;
    }
    
    StatusResult run(std::istream &anInput, std::ostream &anOutput) {
      StatusResult theResult;
      
      Config::getTimer().reset();

      while(theResult) {
        std::string theCommand(readCommand(anInput));
        if(theCommand.length()) {
          std::stringstream theStream(theCommand);
          anOutput << theCommand << "\n";
          theResult=app.handleInput(theStream, [&](View &aView) {
            aView.show(anOutput);
            anOutput << std::endl;
          });
          if(theResult.error != Errors::userTerminated) {
              OptString errView = app.getError(theResult);
              if (errView.has_value())
                  anOutput << errView.value();
          }
        }
        else break;
      }
      anOutput << "Elapsed: " << std::fixed
        << Config::getTimer().elapsed() << "\n";
      if(Errors::userTerminated==theResult.error) {
        theResult=Errors::noError;
      }
      return theResult;
    }
    
  protected:
    AppController &app;
  };

}

#endif /* ScriptRunner_h */
