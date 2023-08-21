//
//  Filters.cpp
//  Datatabase5
//
//  Created by rick gessner on 3/5/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//

#include "Filters.hpp"

namespace ECE141 {
  //--------------------------------------------------------------
  using Comparitor = bool (*)(Value &aLHS, Value &aRHS);

  bool equals(Value &aLHS, Value &aRHS) {
    bool theResult=false;
    
    std::visit([&](auto const &aLeft) {
      std::visit([&](auto const &aRight) {
        theResult=isEqual(aLeft,aRight);
      },aRHS);
    },aLHS);
    return theResult;
  }

  static std::map<Operators, Comparitor> comparitors {
    {Operators::equal_op, equals},
    //STUDENT: Add more for other operators...
  };

  bool Expression::operator()(KeyValues& aList) {
    //STUDENT: Add code here to evaluate the expression...
    return false;
  }
  //--------------------------------------------------------------

  Operand& Operand::setVarChar(const std::string& aValue) {
      ttype = TokenType::string;
      dtype = DataTypes::varchar_type;
      value = aValue;
      return *this;
  }
  Operand& Operand::setAttribute(Token& aToken, DataTypes aType){
      ttype = aToken.type;
      dtype = aType;
      name = aToken.data;
      return *this;
  }
  Operand& Operand::setNumber(Token& aToken){
      ttype = aToken.type;
      dtype = Helpers::getTypeForKeyword(aToken.keyword);
      if (aToken.data.find('.') != std::string::npos) {
          value = std::stof(aToken.data);
      }
      else value = std::stoi(aToken.data);
      return *this;
  }
  //--------------------------------------------------------------
  /*Condition& Condition::setAttribute(Token& aToken, DataTypes aType) {
      keyword = aToken.keyword;
      dtype = aType;
      value = aToken.data;
      return *this;
  }
  */

  //--------------------------------------------------------------
  template <typename T, size_t aSize>
  bool in_array(const T(&array)[aSize], const T& value) {
      for (size_t thePos = 0; thePos < aSize; ++thePos) {
          if (array[thePos] == value) {
              return true;
          }
      }
      return false;
  }

  // USE: parse table name (identifier) with (optional) alias...
  StatusResult ParseHelper::parseTableName(TableName& aTableName) {
      Keywords theKeywords[] = { Keywords::table_kw, Keywords::database_kw, Keywords::into_kw, Keywords::from_kw };
      StatusResult theResult(Errors::invalidTableName, tokenizer.prevPos());
      Token& theToken = tokenizer.previous(); //get token (should be identifier
      if (in_array<Keywords>(theKeywords, theToken.keyword)) {
          theToken = tokenizer.current();
          if (TokenType::identifier == theToken.type) {
              aTableName.table = theToken.data;
              theResult.error = Errors::noError;
              tokenizer.next(); //skip ahead...
              if (tokenizer.skipIf(Keywords::as_kw)) { //try to skip 'as' for alias...
                  Token& theToken = tokenizer.current();
                  aTableName.alias = theToken.data; //copy alias...
                  tokenizer.next(); //skip past alias...
              }
          }
      }
      return theResult;
  }

  StatusResult ParseHelper::parseTableField(Tokenizer& aTokenizer, TableField& aField) {
      StatusResult theResult{ Errors::identifierExpected };
      Token& theToken = aTokenizer.current(); //identifier name?
      if (TokenType::identifier == theToken.type) {
          aField.fieldName = theToken.data;
          aTokenizer.next();
          if (aTokenizer.skipIf(Operators::dot_op)) {
              theToken = aTokenizer.current();
              if (TokenType::identifier == theToken.type) {
                  aTokenizer.next(); //yank it...
                  aField.table = aField.fieldName;
                  aField.fieldName = theToken.data;
              }
          }
          theResult.error = Errors::noError;
      }
      return theResult;

  }


  // USE: gets properties following the type in an attribute decl...
  StatusResult ParseHelper::parseAttributeOptions(Attribute& anAttribute) {
      bool          options = true;
      StatusResult  theResult;
      char          thePunct[] = { ")," }; //removed semi?

      while (theResult && options && tokenizer.more()) {
          Token& theToken = tokenizer.current();
          switch (theToken.type) {
          case TokenType::keyword:
              switch (theToken.keyword) {
              case Keywords::auto_increment_kw:
                  anAttribute.setAutoIncrement(true);
                  break;
              case Keywords::primary_kw:
                  anAttribute.setPrimaryKey(true);
                  break;
              case Keywords::not_kw:
                  tokenizer.next();
                  theToken = tokenizer.current();
                  if (Keywords::null_kw == theToken.keyword) {
                      anAttribute.setNullable(false);
                  }
                  else theResult = StatusResult(Errors::syntaxError, tokenizer.pos());
                  break;
              case Keywords::unique_kw:
                  anAttribute.setUnique(true);
                  break;
              default: break;
              }
              break;

          case TokenType::punctuation: //fall thru...
              options = !in_array<char>(thePunct, theToken.data[0]);
              if (semicolon == theToken.data[0])
                  theResult = StatusResult(Errors::syntaxError, tokenizer.pos());
              break;

          default:
              options = false;
              theResult = StatusResult(Errors::syntaxError, tokenizer.pos());
          } //switch
          if (theResult && tokenizer.more()) tokenizer.next(); //skip ahead...
      } //while
      return theResult;
  }

  //USE : parse an individual attribute (name type [options])
  StatusResult ParseHelper::parseAttribute(Attribute& anAttribute) {
      StatusResult theResult;

      if (tokenizer.more()) {
          Token& theToken = tokenizer.current();
          if (theToken.type == TokenType::identifier) {
              anAttribute.setName(theToken.data);
              tokenizer.next();
              theToken = tokenizer.current();
          }
          else
              goto error_handling;

          if (Helpers::isDatatype(theToken.keyword)) {
              DataTypes theType = Helpers::getTypeForKeyword(theToken.keyword);
              anAttribute.setDataType(theType);
              tokenizer.next();

              if (DataTypes::varchar_type == theType) {
                  if ((tokenizer.skipIf(left_paren))) {
                      theToken = tokenizer.current();
                      tokenizer.next();
                      if ((tokenizer.skipIf(right_paren))) {
                          anAttribute.setSize(atoi(theToken.data.c_str()));
                          // return theResult;
                      }
                  }
              }

              if (theResult) {
                  theResult = parseAttributeOptions(anAttribute);
                  if (theResult) {
                      if (!anAttribute.isValid()) {
                          theResult = StatusResult(Errors::invalidAttribute, tokenizer.pos());
                      }
                  }
              }

          } //if
          else theResult = StatusResult(Errors::unknownDatatype, tokenizer.pos());
      } //if
      return theResult;

  error_handling:
      theResult = StatusResult(Errors::illegalAttributeIdentifier, tokenizer.pos());
      return theResult;
  }

  //USE: parse a comma-sep list of (unvalidated) identifiers;
  //     AUTO stop if keyword (or term)
  StatusResult ParseHelper::parseIdentifierList(StringList& aList) {
      StatusResult theResult;

      while (theResult && tokenizer.more()) {
          Token& theToken = tokenizer.current();
          if (TokenType::identifier == tokenizer.current().type) {
              aList.push_back(theToken.data);
              tokenizer.next(); //skip identifier...
              tokenizer.skipIf(comma);
          }
          else if (theToken.type == TokenType::keyword) {
              break; //Auto stop if we see a keyword...
          }
          else if (tokenizer.skipIf(right_paren)) {
              break;
          }
          else if (semicolon == theToken.data[0]) {
              break;
          }
          else theResult = StatusResult(Errors::syntaxError, tokenizer.pos());
      }
      return theResult;
  }

  //** USE: get a list of values (identifiers, strings, numbers...)
  StatusResult ParseHelper::parseValueList(StringList& aList) {
      StatusResult theResult;

      while (theResult && tokenizer.more()) {
          Token& theToken = tokenizer.current();
          if (TokenType::identifier == theToken.type || TokenType::number == theToken.type) {
              aList.push_back(theToken.data);
              tokenizer.next(); //skip identifier...
              tokenizer.skipIf(comma);
          }
          else if (tokenizer.skipIf(right_paren)) {
              break;
          }
          else theResult = StatusResult(Errors::syntaxError, tokenizer.pos());
      }
      return theResult;
  }

  bool isDotOperator(const Token& aToken) {
      if (TokenType::operators == aToken.type) {
          return aToken.op == Operators::dot_op;
      }
      return false;
  }

  const Attribute* getAttribute(Schema& aSchema, const std::string& aName, const std::string& aFieldName) {
      Schema* theSchema = &aSchema;
      if (aSchema.getName() != aName) {
          //theSchema=aSchema.getDatabase().getSchema(aName);
      }
      return theSchema ? theSchema->getAttribute(aFieldName) : nullptr;
  }


  //where operand is field, number, string...
  StatusResult ParseHelper::parseOperand(Schema& aSchema, Operand& anOp) {
      StatusResult theResult;
      Token& theToken = tokenizer.current();
      if (TokenType::identifier == theToken.type) {
          Token& theNext = tokenizer.peek();
          std::string theEntityName(aSchema.getName());
          if (isDotOperator(theNext)) {
              theEntityName = theToken.data;
              tokenizer.next(2);
              theToken = tokenizer.current();
          }
          if (auto theAttr = getAttribute(aSchema, theEntityName,
              theToken.data)) {
              anOp.setAttribute(
                  theToken, theAttr->getType());
          }
          else if (theToken.type == TokenType::string) anOp.setVarChar(theToken.data);
          else theResult = StatusResult(Errors::unknownAttribute, tokenizer.pos());
      }
      else if (TokenType::number == theToken.type) {
          anOp.setNumber(theToken);
      }
      else theResult = StatusResult(Errors::syntaxError, tokenizer.pos());
      if (theResult) tokenizer.next();
      return theResult;
  }

  StatusResult ParseHelper::parseOperator(Operators& anOp) {
      static Operators gOps[] = {
        Operators::equal_op, Operators::notequal_op,
        Operators::lt_op, Operators::lte_op,
        Operators::gt_op, Operators::gte_op
      };

      StatusResult theResult{ Errors::operatorExpected };
      Token& theToken = tokenizer.current();
      if (in_array<Operators>(gOps, theToken.op)) {
          anOp = theToken.op;
          tokenizer.next();
          theResult.error = Errors::noError;
      }
      return theResult;
  }

  StatusResult ParseHelper::parseExpression(Schema& aSchema,
      Expression& anExpr) {
      StatusResult theResult;

      if ((theResult = parseOperand(aSchema, anExpr.lhs))) {
          if ((theResult = parseOperator(anExpr.op))) {
              theResult = parseOperand(aSchema, anExpr.rhs);
          }
          else theResult = StatusResult(Errors::operatorExpected, tokenizer.pos());
      }
      return theResult;
  }
  // not sure what this is for
  //read a comma-sep list of expressions...
  StatusResult ParseHelper::parseAssignments(Expressions& aList, Schema& aSchema) {
      StatusResult theResult;
      while (theResult && tokenizer.more()) {
          Expression theExpr;
          if ((theResult = parseExpression(aSchema, theExpr))) {
              aList.push_back(std::make_unique<Expression>(theExpr));
              if (!tokenizer.skipIf(',')) {
                  break;
              }
          }
          else theResult = StatusResult(Errors::syntaxError, tokenizer.pos());
      }
      return theResult;
  }
  /*StatusResult ParseHelper::parseCondition(Schema& aSchema, Condition& aCondition) {

  }
  StatusResult ParseHelper::parseRestriction(Condition& aCondition){
      
  }
  */
  //--------------------------------------------------------------
  Filters::Filters()  {}
  
  Filters::Filters(const Filters &aCopy)  {
      for (const auto& theExpress : aCopy.expressions)
          expressions.push_back(std::make_unique<Expression>(*theExpress));
  }
  
  Filters::~Filters() {}

  Filters& Filters::add(Expression *anExpression) {
    expressions.push_back(std::make_unique<Expression>(*anExpression));
    return *this;
  }
    
  //compare expressions to row; return true if matches
  bool Filters::matches(KeyValues &aList) const {
    
    //STUDENT: You'll need to add code here to deal with
    //         logical combinations (AND, OR, NOT):
    //         like:  WHERE zipcode=92127 AND age>20
    
    for (auto& theExpr : expressions) {
      if(!(*theExpr)(aList)) {
        return false;
      }
    }
    
    return true;
  }
 
    
  //STUDENT: Add validation here...
  bool validateOperands(Operand &aLHS, Operand &aRHS, Schema &aSchema) {
    if(TokenType::identifier==aLHS.ttype) { //most common case...
      //STUDENT: Add code for validation as necessary
      return true;
    }
    else if(TokenType::identifier==aRHS.ttype) {
      //STUDENT: Add code for validation as necessary
      return true;
    }
    return false;
  }
  // boolean conditions
  bool isValidOperand(Token &aToken) {
    //identifier, number, string...
      switch (aToken.type) {
      case TokenType::identifier:
      case TokenType::number:
      case TokenType::string:
      case TokenType::timedate:
          return true;
      default:
          break;
      }
      return false;
  }
  // 2-word conditions
  bool isValidCondition(Token& aToken) {
      switch (aToken.keyword) {
      case Keywords::order_kw:
      case Keywords::group_kw:
          return true;
      default:
          break;
      }
      return false;
  }

  // 1-word conditions
  bool isValidRestriction(Token& aToken) {
      switch (aToken.keyword) {
      case Keywords::limit_kw:
          return true;
      default:
          break;
      }
      return false;
  }
  //STUDENT: This starting point code may need adaptation...
  StatusResult Filters::parse(Tokenizer &aTokenizer,Schema &aSchema) {
    StatusResult  theResult;
    ParseHelper theHelper(aTokenizer);
    while(theResult && (2<=aTokenizer.remaining())) {
       Token& theToken = aTokenizer.current();
       if (isValidOperand(theToken)) {
           Expression theExpr;
           if ((theResult = theHelper.parseExpression(aSchema, theExpr))) {
               expressions.push_back(std::make_unique<Expression>(theExpr));
               aTokenizer.next();
               //add logic to deal with bool combo logic...
           }
       }
     /*   else if (isValidCondition(theToken)) {
            Condition theCondition(theToken.keyword);
            aTokenizer.next(2);
            if ((theResult = theHelper.parseCondition(aSchema, theCondition))) {
                conditions.push_back(std::make_unique<Condition>(theCondition));
            }
        }
        else if (isValidRestriction(theToken)) {
            Condition theCondition(theToken.keyword);
            aTokenizer.next();
            if ((theResult = theHelper.parseRestriction(theCondition))) {
                conditions.push_back(std::make_unique<Condition>(theCondition));
            }
        }
        */
      else break;
    }
    return theResult;
  }

}

