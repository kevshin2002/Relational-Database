//
//  TestManually.hpp
//  PA1
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef TestManually_h
#define TestManually_h

#include <sstream>
#include "AppController.hpp"

using namespace ECE141;

bool doManualTesting() {
  ECE141::AppController   theApp(std::cout);
  ECE141::StatusResult  theResult{};

  std::string theUserInput;
  bool running=true;
  do {
    std::cout << "\n> ";
    if(std::getline(std::cin, theUserInput)) {
      if(theUserInput.length()) {
        Config::getTimer().reset();
        std::stringstream theStream(theUserInput);
        theResult=theApp.handleInput(theStream, [&](View &aView) {
          aView.show(std::cout);
          std::cout << "\nElapsed: " << std::fixed
            << Config::getTimer().elapsed() << std::endl;
        });
        if(theResult==Errors::userTerminated) {
          running=false;
        }
        else if(!theResult) {
          std::cout << "Error: " << theApp.getError(theResult).value() << "\n";
        }
      }
    }
  }
  while (running);
  return theResult;
}

#endif /* TestManually_h */
