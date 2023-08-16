//
//  Filters.hpp
//  RGAssignment5
//
//  Created by rick gessner on 4/4/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//

#ifndef Filters_hpp
#define Filters_hpp

#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <limits>

#include "../Utilities/Tokenizer/Tokenizer.hpp"
#include "../Database/Storage/Table/Schema.hpp"
#include "../Utilities/Helpers.hpp"
#include "../Misc/Types/keywords.hpp"
#include "Compare.hpp"


namespace ECE141 {
  using StringList = std::vector<std::string>;
  class Entity;
  class Schema;
  struct Operand {
      Operand() : ttype(TokenType::unknown), dtype(DataTypes::no_type) {}
      Operand(std::string& aName, TokenType aType, Value& aValue, size_t anId = 0) : ttype(aType), dtype(DataTypes::varchar_type), name(aName), value(aValue), schemaId(anId) {}
      Operand& setVarChar(const std::string& aValue);
      Operand& setAttribute(Token& aToken, DataTypes aType);
      Operand& setNumber(Token& aToken);

      TokenType   ttype; //is it a field, or const (#, string)...
      DataTypes   dtype;
      std::string name;  //attr name
      Value       value;
      size_t      schemaId;
  };
  
  //---------------------------------------------------
               
  struct Expression {
      Expression(const Operand& aLHSOperand = {}, const Operators anOp = Operators::unknown_op, const Operand& aRHSOperand = {}) : lhs(aLHSOperand), rhs(aRHSOperand), op(anOp), logic(Logical::no_op) {}
      Expression(const Expression& anExpr) : lhs(anExpr.lhs), rhs(anExpr.rhs), op(anExpr.op), logic(anExpr.logic) {}

      bool operator()(KeyValues& aList);

      Operand     lhs;  //id
      Operand     rhs;  //usually a constant; maybe a field...
      Operators   op;   //=     //users.id=books.author_id
      Logical     logic; //and, or, not...

  };

  using Expressions = std::vector<std::unique_ptr<Expression>>;
  struct TableName {
      TableName() {}
      TableName(const std::string& aTableName, const std::string& anAlias = "") : table(aTableName), alias(anAlias) {}
      TableName(const TableName& aCopy) : table(aCopy.table), alias(aCopy.alias) {}
      TableName& operator=(const std::string& aName) {
          table = aName;
          return *this;
      };
      operator const std::string() { return table; }

      std::string table;
      std::string alias;
  };

  struct TableField {
      const std::string_view& getTableName() const { return table; }
      const std::string_view& getTableField() const { return fieldName; }

      std::string table;
      std::string fieldName;
  };

  struct ParseHelper {

      ParseHelper(Tokenizer& aTokenizer) : tokenizer(aTokenizer) {}

      StatusResult parseTableName(TableName& aTableName);
      StatusResult parseTableField(Tokenizer& aTokenizer, TableField& aField);
      StatusResult parseAttributeOptions(Attribute& anAttribute);

      StatusResult parseAttribute(Attribute& anAttribute);

      StatusResult parseIdentifierList(StringList& aList);

      StatusResult parseAssignments(Expressions& aList, Schema&);

      StatusResult parseValueList(StringList& aList);

      StatusResult parseOperator(Operators& anOp);
      StatusResult parseOperand(Schema&, Operand&);
      StatusResult parseExpression(Schema&, Expression&);

      Tokenizer& tokenizer;
  };

  class Filters {
  public:

      Filters();
      Filters(const Filters& aFilters);
      ~Filters();

      size_t        getCount() const { return expressions.size(); }
      bool          matches(KeyValues& aList) const;
      Filters& add(Expression* anExpression);
      Expressions& getExpressions() { return expressions; }
      StatusResult  parse(Tokenizer& aTokenizer, Schema& aSchema);

  protected:
      Expressions   expressions;
  };
 
}

#endif /* Filters_hpp */
