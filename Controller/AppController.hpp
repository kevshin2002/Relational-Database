//
//  AppController.hpp
//  Database5
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#ifndef AppController_hpp
#define AppController_hpp

#include "Processors/DBProcessor.hpp"

namespace ECE141 {
  class AppController : public AppProcessor {
  public:
    AppController();
    ~AppController();
    StatusResult        handleInput(std::istream& anInput,  ViewListener aViewer);
    bool                isRunning() const { return running; }
     
    bool                isProcessable(Keywords& aKeyword) const override;
    AppProcessor*       findHandler(Tokenizer& aTokenizer) override;
    Statement*          makeStatement(Tokenizer& aTokenizer, AppController* anAppController) override;
    StatusResult        run(Statement* aStatement, ViewListener aViewer) override;

    bool                holdDB(UniqueDatabase& aDB);
    bool                releaseDB();
    UniqueDatabase& getDB() { return db; }

  protected:
    OptString           getError(StatusResult& aResult) const;

  private:
    char terminator = ';';
    bool running = false;
    UniqueDatabase db;
    AppProcessor* next = nullptr;
  };
  
}

#endif /* AppController_hpp */
