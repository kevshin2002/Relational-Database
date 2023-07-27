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
#include "../Database/Database.hpp"
#include "../Utilities/Tokenizer/Tokenizer.hpp"
#include "../Statements/Statement.hpp"
#include "../View/Views/StringView.hpp"

namespace ECE141 {
  class AppController {
  public:
    
    AppController();
    virtual ~AppController();

      //app api...    
    virtual StatusResult      handleInput(std::istream &anInput,  ViewListener aViewer);
    virtual bool              isProcessable(Keywords& aKeyword) const;
    virtual AppController*    findHandler(Tokenizer& aTokenizer);
    virtual StatusResult      run(Statement* aStatement, ViewListener aViewer);
    virtual Statement*        makeStatement(Tokenizer& aTokenizer, AppController* anAppController);

  protected:
    bool                      isRunning() const {return running;}
    OptString                 getError(StatusResult& aResult) const;

      bool running;
      UniqueDB db;
      AppController* next;
  };
  
}

#endif /* AppController_hpp */
