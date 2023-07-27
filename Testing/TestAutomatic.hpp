//
//  TestAutomatic.hpp
//  Database
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef TestAutomatic_h
#define TestAutomatic_h

#include <iostream>
#include <sstream>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <random>
#include <functional>
#include <filesystem>
#include <stack>

#include "../Misc/Types/Errors.hpp"
#include "../Utilities/FolderReader.hpp"
#include "TestSequencer.hpp"
#include "ScriptRunner.hpp"

using namespace ECE141;
namespace fs=std::filesystem;

namespace ECE141 {
  
  using Validate    = std::function<bool(const std::string &aBuffer)>;
  using TestCall    = std::function<bool(std::stringstream &anOutput)>;
  using TestCalls   = std::map<std::string, TestCall>;
  using InputLine   = std::pair<std::string_view, size_t>;
  using StringList = std::vector<std::string>;
  using LineList    = std::initializer_list<InputLine>;

  const std::string_view theVersion{"Tester(1.1)"};

  using CountList = std::vector<int>;

  //------------------------------------

  enum Commands {unknown, alter, createDB, createTable, describe,
    delet, dropDB, dropTable, dumpDB, insert, select, showDBs,
    showTables, showIndex, showIndexes, update, useDB};

  struct CommandCount {
    Commands  command;
    int       count;
    char      cmp='=';
    
    bool compare(const CommandCount &anItem) {
      if(command==anItem.command) {
        if('>'==cmp) return anItem.count>count;
        return anItem.count==count;
      }
      return false;
    }
  };

  using Responses = std::vector<CommandCount>;
  using ExpectList = std::initializer_list<CommandCount>;

  class Expected {
  public:
    Expected(const ExpectList &aList) : expected(aList) {}
    
    bool operator==(const Responses &aResponses) {
      size_t theSize=expected.size();
      if(theSize && aResponses.size()==theSize) {
        for(size_t i=0;i<theSize;i++) {
          if(!expected[i].compare(aResponses[i])) {
            return false;
          }
        }
        return true;
      }
      return false;
    }
    
  protected:
    Responses expected;
  };

  //------------------------------------

  class TestAutomatic {
  public:
    
    ~TestAutomatic() {
      std::cout << "---------- " << theVersion << " ----------\n";
    }

    bool compileTest(std::ostream &anOutput) {return true;}

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
    
    bool containsCmdList(const std::string &aBuffer) {
      return true;
    }
    
    bool aboutTest(std::stringstream &anOutput) {
      std::string theScript{"about;"};
      std::stringstream theInput(theScript);
      return scriptTest(theInput, anOutput, [&](const std::string &aBuffer) {
        return containsCmdList(aBuffer);
      });
    } //about

    //---------------------------------------
    
    bool dojoTest(std::stringstream &anOutput) {
      std::cout << "I'm doing the dojo test!\n";
      return true;
    }

    //----------------------------------------------
    
    bool hasHelp(const std::string &aString) {
      static std::vector<const char*> theCmds{
        "create database","show databases","drop database","use database",
        "about","version","quit"
      };
      for(auto thePhrase : theCmds) {
        if (aString.find(thePhrase) == std::string::npos) {
          return false;
        }
      }
      return true;
    }
    
    bool helpTest(std::stringstream &anOutput) {
      std::string theScript{"help;"};
      std::stringstream theInput(theScript);
      return scriptTest(theInput, anOutput, [&](const std::string &aBuffer) {
        return hasHelp(aBuffer);
      });
    }
    
    //----------------------------------------------

    bool quitTest(std::stringstream &anOutput) {
      std::string theScript{"quit;"};
      std::stringstream theInput(theScript);
      return scriptTest(theInput, anOutput, [&](const std::string &aBuffer) {
        return matches(aBuffer, {{"quit",4},{"DB::141 is shutting down",24}});
      });
    } //quit
    
    //----------------------------------------------

    bool scriptTest(std::istream &anInput, std::stringstream &anOutput,
                      Validate aValidator) {
      ECE141::AppController theApp;
      ScriptRunner        theRunner(theApp);
      theRunner.run(anInput, anOutput);
      auto theBuffer{anOutput.str()};
      return aValidator(theBuffer);
    }

    //----------------------------------------------
    
    bool versionTest(std::stringstream &anOutput) {
      std::string theScript{"version;"};
      std::stringstream theInput(theScript);
      return scriptTest(theInput,anOutput,[&](const std::string &aBuffer) {
        return matches(aBuffer, {{"version",7}, {"Version:",10}});
      });
    }
    
    //---------------------------------------------------
    
    enum states {wasUnknown, wasAlter, wasCreate, wasDescribe,
        wasDelete, wasDrop, wasDump, wasInsert, wasSelect,
        wasShow, wasUpdate, wasUse};
              
    //validates output of DBCommand test..
    size_t analyzeOutput(std::istream &aStream, Responses &aResults) {
      std::stack<Commands> theStack; //tracking open state...

      static KWList createDB{Keywords::create_kw,Keywords::database_kw};
      static KWList changedDB{Keywords::database_kw,Keywords::changed_kw};
      static KWList showDBs{Keywords::show_kw,Keywords::databases_kw};
      static KWList dumpDB{Keywords::dump_kw,Keywords::database_kw};
      static KWList dropDB{Keywords::drop_kw,Keywords::database_kw};
      static KWList createTable{Keywords::create_kw,Keywords::table_kw};
      static KWList showTables{Keywords::show_kw,Keywords::tables_kw};
      static KWList showIndex{Keywords::show_kw,Keywords::index_kw};
      static KWList showIndexes{Keywords::show_kw,Keywords::indexes_kw};
      static KWList dropTable{Keywords::drop_kw,Keywords::table_kw};
      static KWList insertInto{Keywords::insert_kw,Keywords::into_kw};

      Tokenizer theTokenizer(aStream);
      if(theTokenizer.tokenize()) {
        TestSequencer theSeq(theTokenizer);
        int theValue{0};
        while(theTokenizer.more()) {
          if(theSeq.clear().nextIs(createDB)) {
            if(theSeq.skipPast(Keywords::query_kw).skip(2)) {
              theSeq.getNumber(theValue).skip(7);
              aResults.push_back({Commands::createDB,theValue});
            }
          }
          else if(theSeq.clear().nextIs(showDBs)) {
            if(theSeq.skip(1)) {
              theTokenizer.skipTo(TokenType::number);
              theSeq.getNumber(theValue).skipPast(')');
              aResults.push_back({Commands::showDBs,theValue});
            }
          }
          else if(theSeq.clear().nextIs({Keywords::use_kw})) {
            if(theSeq.skip(2).nextIs(changedDB).skipPast(')')) {
              aResults.push_back({Commands::useDB, 0});
            }
          }
          else if(theSeq.clear().nextIs(dropDB)) {
            if(theSeq.skipPast(Keywords::query_kw).skip(2)) {
              theSeq.getNumber(theValue).skipPast(')');
              aResults.push_back({Commands::dropDB,theValue});
            }
          }
          else if(theSeq.clear().nextIs(dumpDB)) {
            if(theTokenizer.skipTo(Keywords::rows_kw)) {
              auto theToken=theTokenizer.peek(-1);
              theValue=std::stoi(theToken.data);
              theSeq.skip(7);
            }
            aResults.push_back({Commands::dumpDB,theValue});
          }
          else if(theSeq.clear().nextIs(createTable)) {
            if(theSeq.skipPast(';').nextIs({Keywords::query_kw}).skip(2)) {
              theSeq.getNumber(theValue).skip(7);
              aResults.push_back({Commands::createTable,theValue});
            }
          }
          else if(theSeq.clear().nextIs(showTables)) {
            theTokenizer.skipTo(TokenType::number);
            theSeq.getNumber(theValue).skipPast(')');
            aResults.push_back({Commands::showTables,theValue});
          }
          else if(theSeq.clear().nextIs(showIndex)) {
            if(theTokenizer.skipTo(Keywords::rows_kw)) {
              auto theToken=theTokenizer.peek(-1);
              theValue=std::stoi(theToken.data);
              theSeq.skip(7);
            }
            aResults.push_back({Commands::showIndex,theValue});
          }
          else if(theSeq.clear().nextIs(showIndexes)) {
            theTokenizer.skipTo(TokenType::number);
            theSeq.getNumber(theValue).skipPast(')');
            aResults.push_back({Commands::showIndexes,theValue});
          }
          else if(theSeq.clear().nextIs(dropTable)) {
            theTokenizer.skipTo(TokenType::number);
            theSeq.getNumber(theValue).skipPast(')');
            aResults.push_back({Commands::dropTable,theValue});
          }
          else if(theSeq.clear().nextIs(insertInto)) {
            if(theSeq.skipPast(';').nextIs({Keywords::query_kw}).skip(2)) {
              theSeq.getNumber(theValue).skip(7);
              aResults.push_back({Commands::insert,theValue});
            }
          }
          else if(theTokenizer.skipIf(Keywords::describe_kw)) {
            if(theTokenizer.skipTo(Keywords::rows_kw)) {
              auto theToken=theTokenizer.peek(-1);
              theValue=std::stoi(theToken.data);
              aResults.push_back({Commands::describe,theValue});
              theSeq.clear().skipPast(')');
            }
          }
          else if(theSeq.clear().nextIs({Keywords::select_kw})) {
            if(theTokenizer.skipTo(Keywords::rows_kw)) {
              auto theToken=theTokenizer.peek(-1);
              theValue=std::stoi(theToken.data);
              aResults.push_back({Commands::select,theValue});
              theSeq.clear().skipPast(')');
            }
          }
          else if(theSeq.clear().nextIs({Keywords::update_kw})) {
            if(theTokenizer.skipTo(Keywords::rows_kw)) {
              auto theToken=theTokenizer.peek(-1);
              theValue=std::stoi(theToken.data);
              aResults.push_back({Commands::update,theValue});
              theSeq.clear().skipPast(')');
            }
          }
          else if(theSeq.clear().nextIs({Keywords::delete_kw})) {
            if(theTokenizer.skipTo(Keywords::rows_kw)) {
              auto theToken=theTokenizer.peek(-1);
              theValue=std::stoi(theToken.data);
              aResults.push_back({Commands::delet,theValue});
              theSeq.clear().skipPast(')');
            }
          }
          else theTokenizer.next(); //skip...
        }
      }
    return aResults.size();
  }
            
    bool hasFiles(const StringList &aFilelist) {
      for(auto theFile : aFilelist) {
        std::string thePath=Config::getDBPath(theFile);
        std::ifstream theStream(thePath);
        if(!theStream) return false;
      }
      return true;
    }
    
    std::string getRandomDBName(char aChar) {
      uint32_t theCount=rand() % 99999;
      return std::string("testdb_"+std::to_string(theCount+1000)+aChar);
    }
    
    bool compareCounts(CountList &aList1, CountList &aList2, const size_t aPos) {
      bool theResult=true;
      for(size_t i=0;i<aList1.size();i++) {
        if(theResult) {
          theResult=i==aPos ? aList1[i]<aList2[i] : aList1[i]==aList2[i];
        }
      }
      return theResult;
    }
    
    size_t countDBFiles() {
      std::string thePath=Config::getStoragePath();
      FolderReader theReader(thePath.c_str());
      size_t theCount{0};
      theReader.each(Config::getDBExtension(),
                     [&](const std::string &aName) {
        theCount++;
        return true;
      });
      return theCount;
    }
    
    bool deleteFiles(StringList &aFileList) {
      for(auto &theFile : aFileList) {
        fs::remove(theFile);
      }
      return true;
    }
      
   //----------------------------------------------
    
    bool createFile(const std::string &aPath, const std::string &aName) {
      std::string temp(aPath);
      temp+='/';
      temp+=aName;
      std::fstream output(temp, std::fstream::out | std::fstream::trunc);
      output << "some text\n";
      return true;
    }
    
    //----------------------------------------------

    bool doReaderTest() {
      std::string thePath=Config::getStoragePath();
      createFile(thePath,"test1.txt");
      
      FolderReader theReader(thePath.c_str());
      size_t theCount{0};
      theReader.each(".txt",[&](const std::string &aName) {
        theCount++;
        return true;
      });
      return theCount>0;
    }
    //----------------------------------------------
    
    bool doCreateDBTests(const StringList &aFileList, Responses &aResponses,
                         std::stringstream &anOutput) {
      
      std::stringstream theCmds;
      theCmds << "CREATE DATABASE "+aFileList[0] << ";\n ";
      theCmds << "create database "+aFileList[1] << ";\n ";
      theCmds << "CrEaTe dAtABaSe "+aFileList[2] << ";\n ";
      theCmds << "use "+aFileList[0] << ";\n";
      theCmds << "shoW databaseS;\n";
      
      auto theStr=theCmds.str();
      std::stringstream theOutput1;
      
      theCmds >> std::noskipws; //make sure to include ws...
      
      bool theResult=scriptTest(theCmds, theOutput1,
                                  [&](const std::string &aBuffer) {
        if(hasFiles(aFileList)) {
          auto temp=theOutput1.str();
          std::stringstream theInput(temp);
          anOutput << temp; //show user...
          auto theCount=analyzeOutput(theInput,aResponses);
          Expected theExpected({
            {Commands::createDB,1}, {Commands::createDB,1},
            {Commands::createDB,1}, {Commands::useDB,0}, {Commands::showDBs,2,'>'}
          });
          return theCount && theExpected==aResponses;
        }
        return false;
      }); //script test
      return theResult;
    }
    
    bool doDropAndShowTests(const StringList &aFileList, int aPrevDBCount,
                            std::stringstream &anOutput) {
      
      std::stringstream theCmds, theOut;
      
      theCmds << "DRop dataBASE "+aFileList[1] << ";\n ";
      theCmds << "shoW databaseS;\n";
      theCmds << "dump database "+aFileList[0] << ";\n ";
      theCmds << "drop database "+aFileList[0] << ";\n ";
      theCmds << "drop database "+aFileList[2] << ";\n ";
      
      theCmds >> std::noskipws; //make sure to include ws...

      bool theResult=scriptTest(theCmds, theOut,[&](const std::string &aBuf) {
        auto temp=theOut.str();
        std::stringstream theOutput(temp);
        anOutput << temp << "\n"; //capture!
        
        Responses theResponses;
        if((theResult=analyzeOutput(theOutput,theResponses))) {
          Expected theExpected({
            {Commands::dropDB,0}, {Commands::showDBs, 1, '>'},
            {Commands::dumpDB, -1,'>'}, {Commands::dropDB,0},
            {Commands::dropDB,0}
          });
          if(theExpected==theResponses) {
            return true;
          }
        }
        return false;
      });

      return theResult;
    }
    
    bool dbCommandsTest(std::stringstream &anOutput) {      
      std::vector<std::string> theFiles;
      const char theType='A';
      theFiles.push_back(getRandomDBName(theType));
      theFiles.push_back(getRandomDBName(theType));
      theFiles.push_back(getRandomDBName(theType));
      bool theResult{false};
      if(theFiles.size()) {
        Responses theResponses;
        theResult=doCreateDBTests(theFiles, theResponses, anOutput);
        if(theResult) {
          int theDBCount=theResponses[4].count;
          theResult=doDropAndShowTests(theFiles, theDBCount, anOutput);
        }
        deleteFiles(theFiles);
      }
      return theResult;
    }

    bool cacheCommandTest(std::stringstream &anOutput) {
      return false;
    }

    bool filterCommandTest(std::stringstream &anOutput) {
      return false;
    }
    
    bool indexCommandTest(std::stringstream &anOutput) {
      return false;
    }

    bool joinCommandTest(std::stringstream &anOutput) {
      return false;
    }

    bool mutateCommandTest(std::stringstream &anOutput) {
      return false;
    }

    bool rowCommandTest(std::stringstream &anOutput) {
      return false;
    }

    bool selectCommandTest(std::stringstream &anOutput) {
      return false;
    }

    bool tableCommandTest(std::stringstream &anOutput) {
      return false;
    }

    
  }; //TestAutomatic...

}


#endif /* TestAutomatic_h */
