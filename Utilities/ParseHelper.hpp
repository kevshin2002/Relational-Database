//
//  ParseHelper.hpp
//  RGAssignment4
//
//  Created by rick gessner on 4/18/21.
//

#ifndef ParseHelper_hpp
#define ParseHelper_hpp

#include <stdio.h>
#include <vector>
#include "Helpers.hpp"
#include "../Misc/Types/keywords.hpp"
#include "../Utilities/Tokenizer/Tokenizer.hpp"
#include "../Database/Storage/Table/Attribute.hpp"
#include "../Tests/Testing/TestSequencer.hpp"
#include "../Database/Database.hpp"
namespace ECE141 {

  //-------------------------------------------------
  
  class Schema;
  class Expression;
  using StringList = std::vector<std::string>;
  struct TableName {
      TableName(const std::string& aTableName, const std::string& anAlias = "")
          : table(aTableName), alias(anAlias) {}
      TableName(const TableName& aCopy) : table(aCopy.table), alias(aCopy.alias) {}
      TableName& operator=(const std::string& aName) {
          table = aName;
          return *this;
      }
      operator const std::string() { return table; }

      std::string table;
      std::string alias;
  };

  struct ParseHelper {
            
    ParseHelper(Tokenizer &aTokenizer) : tokenizer(aTokenizer) {}
        
    StatusResult parseTableName(TableName &aTableName);
        
    StatusResult parseAttributeOptions(Attribute &anAttribute);

    StatusResult parseAttribute(Attribute &anAttribute);

    StatusResult parseIdentifierList(StringList &aList);


    StatusResult parseValueList(StringList &aList);
            
/*
    StatusResult parseOperator(Operators &anOp);
    StatusResult parseOperand(Entity &, Operand&);
    StatusResult parseExpression(Entity&, Expression&);
    StatusResult parseAssignments(Expressions &aList,
                                  Entity &anEnity);
*/
    Tokenizer &tokenizer;
  };


}

#endif /* ParseHelper_hpp */

