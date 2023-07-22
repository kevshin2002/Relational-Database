//
//  AppProcessor.hpp
//  Database5
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#ifndef AppController_hpp
#define AppController_hpp

#include <stdio.h>
#include "../Utilities/Config.hpp"
#include "Processors/ErrorProcessor.hpp"
#include "../Factories/CommandFactory.hpp"
#include "../Database/Database.hpp"

namespace ECE141 {
  using TokenCache = std::vector<Token>;
  class AppController{
  public:
    
    AppController(std::ostream &anOutput);
    virtual ~AppController();

      //app api...    
    virtual StatusResult  handleInput(std::istream &anInput,  ViewListener aViewer);
    StatusResult          hasSemicolon(Tokenizer& aTokenizer, StatusResult& aResult);
            bool          isRunning() const {return running;}
            void          emptyCache() { cache.clear(); }
            OptString     getError(StatusResult &aResult);

    bool running;

  protected:
      UniqueDB db;
      ErrorProcessor errorProc;
      TokenCache cache;
  };
  
}

#endif /* AppController_hpp */
