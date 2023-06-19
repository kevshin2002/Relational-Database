//
//  main.cpp
//  Database2
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>

#include "TestManually.hpp"
#include "TestAutomatic.hpp"

//----------------------------------------------

int main(int argc, const char * argv[]) {
        
  srand(static_cast<uint32_t>(time(0)));
    
  if(argc>1) {
    
    ECE141::TestAutomatic theTests;

    static ECE141::TestCalls theCalls {
      {"about",   [&](std::stringstream &anOut){
        return theTests.doAboutTest(anOut);}},
      {"compile", [&](std::stringstream &anOut){
        return theTests.doCompileTest(anOut);}},
      {"dojo",    [&](std::stringstream &anOut){
        return theTests.doDojoTest(anOut);}},
      {"quit", [&](std::stringstream &anOut){
        return theTests.doQuitTest(anOut);}},            
      {"version", [&](std::stringstream &anOut){
        return theTests.doVersionTest(anOut);}},
    };
    
    std::string theCmd(argv[1]);
    std::transform(theCmd.begin(), theCmd.end(), theCmd.begin(),
        [](unsigned char c){ return std::tolower(c); });
    
    if(theCalls.count(theCmd)) {
      std::stringstream theOutput;
      bool theResult = theCalls[theCmd](theOutput);
      const char* theStatus[]={"FAIL","PASS"};
      std::cout << theCmd << " test " << theStatus[theResult] << "\n";
      std::cout << "---------------------------------\n" << theOutput.str() << "\n";
    }
    else std::cout << "Unknown test\n";
  }
  else {
    doManualTesting();
  }
  return 0;
}
