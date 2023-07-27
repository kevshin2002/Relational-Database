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

#include "Testing/TestManually.hpp"
#include "Testing/TestAutomatic.hpp"

//----------------------------------------------

int main(int argc, const char * argv[]) {
        
  srand(static_cast<uint32_t>(time(0)));
    
  if(argc>1) {
    
    ECE141::TestAutomatic run;
    static ECE141::TestCalls theCalls {
      {"about", [&](std::stringstream &anOut){
        return run.aboutTest(anOut);}},
      {"compile", [&](std::stringstream &anOut){
        return run.compileTest(anOut);}},
      {"db", [&](std::stringstream &anOut){
        return run.dbCommandsTest(anOut);}},
      {"help",   [&](std::stringstream &anOut){
        return run.helpTest(anOut);}},
      {"dojo",    [&](std::stringstream &anOut){
        return run.dojoTest(anOut);}},      
      {"cache", [&](std::stringstream &anOut){
        return run.cacheCommandTest(anOut);}},
      {"filter", [&](std::stringstream &anOut){
        return run.filterCommandTest(anOut);}},
      {"index", [&](std::stringstream &anOut){
        return run.indexCommandTest(anOut);}},
      {"join", [&](std::stringstream &anOut){
        return run.joinCommandTest(anOut);}},
      {"mutate", [&](std::stringstream &anOut){
        return run.mutateCommandTest(anOut);}},
      {"row", [&](std::stringstream &anOut){
        return run.rowCommandTest(anOut);}},
      {"select", [&](std::stringstream &anOut){
        return run.selectCommandTest(anOut);}},
      {"table", [&](std::stringstream &anOut){
        return run.tableCommandTest(anOut);}},
      {"quit", [&](std::stringstream &anOut){
        return run.quitTest(anOut);}},
      {"version", [&](std::stringstream &anOut){
        return run.versionTest(anOut);}},
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
