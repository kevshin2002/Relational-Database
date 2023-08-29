//
//  Config.hpp
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//
//

#ifndef Config_h
#define Config_h

#include <sstream>
#include "Timer.hpp"
#include <filesystem>

namespace ECE141 {
  
  enum class CacheType : int {block=0, rows, views};

  struct Config {
    static const char* getDBExtension() {return ".db";}
    static const std::string getDirectoryPath() { return std::filesystem::current_path().string(); }

    static const std::string getStoragePath() {
        
      #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        
        //STUDENT: If you're on windows, return a path to folder on your machine...
        return Config::getDirectoryPath() + "/Files";
      
      #elif __APPLE__ || defined __linux__ || defined __unix__

        return std::string("/tmp");
              
      #endif
    }
    static std::string getDBPath(const std::string &aDBName) {
      std::ostringstream theStream;
      theStream << Config::getStoragePath() << "/" << aDBName << Config::getDBExtension();
      return theStream.str();
    }
      
    static Timer& getTimer() {
      static Timer theTimer;
      return theTimer;
    }
    
    static std::string getVersion() {return "0.1";}
    static std::string getMembers() {return "Author: Kevin Shin";}

  };

}

#endif /* Config_h */
