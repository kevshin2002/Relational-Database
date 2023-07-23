//
//  TestAutomatic.hpp
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef TestAutomatic_h
#define TestAutomatic_h

#include "../Controller/AppController.hpp"
#include "ScriptRunner.hpp"
#include "../Misc/Types/Errors.hpp"
#include <sstream>
#include <vector>
#include <functional>

using namespace ECE141;


namespace ECE141 {
  

  using StringMap   = std::map<std::string_view, std::string_view>;
  using Validate    = std::function<bool(const std::string &aBuffer)>;
  using TestCall    = std::function<bool(std::stringstream &anOutput)>;
  using TestCalls   = std::map<std::string, TestCall>;
  using InputLine   = std::pair<std::string_view, size_t>;
  using LineList    = std::initializer_list<InputLine>;

  const std::string_view theVersion{"Tester(1.0)"};

  class TestAutomatic {
  public:
    
    ~TestAutomatic() {
      std::cout << "---------- " << theVersion << " ----------\n";
    }

    bool doCompileTest(std::ostream &anOutput) {return true;}

    //----------------------------------------------
    
    bool matches(const std::string &aBuffer, const LineList &aLines) {
      std::stringstream theInput(aBuffer);
      std::string theLine;
      for(auto &theItem: aLines) {
        if(std::getline(theInput,theLine)) {
          if (theLine.find(theItem.first) != std::string::npos) {
            if(theLine.length()<theItem.second)
              return false;
          }
          else return false;
        }
        else return false;
      }
      return true;
    }
    
    //----------------------------------------------

    bool doAboutTest(std::stringstream &anOutput) {
      std::string theScript{"about;"};
      std::stringstream theInput(theScript);
      return doScriptTest(theInput, anOutput, [&](const std::string &aBuffer) {
        return matches(aBuffer, {{"about;",5},{"Authors:",10}});
      });
    } //about

    //---------------------------------------
    
    bool doDojoTest(std::stringstream &anOutput) {
      std::cout << "I'm doing the dojo test!\n";
      return true;
    }
    
    //----------------------------------------------

    bool doScriptTest(std::istream &anInput,std::stringstream &anOutput,
                      Validate aValidator) {
      ECE141::AppController theApp(anOutput);
      ScriptRunner        theRunner(theApp);
      theRunner.run(anInput, anOutput);
      auto theBuffer{anOutput.str()};
      return aValidator(theBuffer);
    }

    //----------------------------------------------

    bool doQuitTest(std::stringstream &anOutput) {
      std::string theScript{"quit;"};
      std::stringstream theInput(theScript);
      return doScriptTest(theInput, anOutput, [&](const std::string &aBuffer) {
        return matches(aBuffer, {{"quit;",4},{"DB::141 is shutting down",24}});
      });
    } //quit
    
    //----------------------------------------------
    
    bool doVersionTest(std::stringstream &anOutput) {
      std::string theScript{"version;"};
      std::stringstream theInput(theScript);
      return doScriptTest(theInput,anOutput,[&](const std::string &aBuffer) {
        return matches(aBuffer, {{"version;",7}, {"Version:",10}});
      });
    }
    
  };

}


#endif /* TestAutomatic_h */
