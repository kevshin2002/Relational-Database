//
//  Helpers.hpp
//  Database3
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef Helpers_h
#define Helpers_h

#include "../Misc/Types/keywords.hpp"
#include <algorithm>

namespace ECE141 {

  static std::map<std::string, Operators> gExpressionOps = {
    {"=",   Operators::equal_op},
    {"<",   Operators::lt_op},
    {"<=",  Operators::lte_op},
    {">",   Operators::gt_op},
    {">=",  Operators::gte_op},
    {"!=",  Operators::notequal_op},
    {"not", Operators::notequal_op},
  };

  static std::map<std::string, Operators> gOperators = {
    std::make_pair(".", Operators::dot_op),
    std::make_pair("+", Operators::add_op),
    std::make_pair("-", Operators::subtract_op),
    std::make_pair("*", Operators::multiply_op),
    std::make_pair("/", Operators::divide_op),
    std::make_pair("^", Operators::power_op),
    std::make_pair("%", Operators::mod_op),
    std::make_pair("=", Operators::equal_op),
    std::make_pair("!=", Operators::notequal_op),
    std::make_pair("not", Operators::notequal_op),
    std::make_pair("<", Operators::lt_op),
    std::make_pair("<=", Operators::lte_op),
    std::make_pair(">", Operators::gt_op),
    std::make_pair(">=", Operators::gte_op),
    std::make_pair("and", Operators::and_op),
    std::make_pair("or", Operators::or_op),
    std::make_pair("nor", Operators::nor_op),
    std::make_pair("between", Operators::between_op),
  };
  
  static ECE141::Keywords gJoinTypes[] = {
    ECE141::Keywords::cross_kw,ECE141::Keywords::full_kw, ECE141::Keywords::inner_kw,
    ECE141::Keywords::left_kw, ECE141::Keywords::right_kw
  };

  //a list of known functions...
  static std::map<std::string,int> gFunctions = {
    std::make_pair("avg", 10),
    std::make_pair("count", 20),
    std::make_pair("max", 30),
    std::make_pair("min", 40)
  };
  static std::map<Keywords, CommandType> KWToCmdType = {
    std::make_pair(Keywords::about_kw, CommandType::basic_command),
    std::make_pair(Keywords::version_kw, CommandType::basic_command),
    std::make_pair(Keywords::quit_kw, CommandType::basic_command),
    std::make_pair(Keywords::help_kw, CommandType::basic_command),
  };

  static std::map<Keywords, StatementType> statements = {
    std::make_pair(Keywords::about_kw, StatementType::about),
    std::make_pair(Keywords::version_kw, StatementType::version),
    std::make_pair(Keywords::quit_kw, StatementType::quit),
    std::make_pair(Keywords::help_kw, StatementType::help),
  };

  //This (incomplete) map binds keyword-string to Keyword (token)...
  static std::map<std::string_view,  Keywords> gDictionary = {
    std::make_pair("about",     Keywords::about_kw),
    std::make_pair("add",       Keywords::add_kw),
    std::make_pair("all",       Keywords::all_kw),
    std::make_pair("alter",     Keywords::alter_kw),
    std::make_pair("and",       Keywords::and_kw),
    std::make_pair("as",        Keywords::as_kw),
    std::make_pair("asc",       Keywords::asc_kw),
    std::make_pair("avg",       ECE141::Keywords::avg_kw),
    std::make_pair("auto_increment", Keywords::auto_increment_kw),
    std::make_pair("between",   ECE141::Keywords::between_kw),
    std::make_pair("boolean",   ECE141::Keywords::boolean_kw),
    std::make_pair("by",        ECE141::Keywords::by_kw),
    std::make_pair("change",    ECE141::Keywords::change_kw),
    std::make_pair("changed",    ECE141::Keywords::changed_kw),
    std::make_pair("char",      ECE141::Keywords::char_kw),
    std::make_pair("column",    ECE141::Keywords::column_kw),
    std::make_pair("count",     ECE141::Keywords::count_kw),
    std::make_pair("create",    ECE141::Keywords::create_kw),
    std::make_pair("cross",     ECE141::Keywords::cross_kw),
    std::make_pair("current_date",  Keywords::current_date_kw),
    std::make_pair("current_time",  Keywords::current_time_kw),
    std::make_pair("current_timestamp", Keywords::current_timestamp_kw),
    std::make_pair("database",  ECE141::Keywords::database_kw),
    std::make_pair("databases", ECE141::Keywords::databases_kw),
    std::make_pair("datetime",  ECE141::Keywords::datetime_kw),
    std::make_pair("decimal",   ECE141::Keywords::decimal_kw),
    std::make_pair("desc",      ECE141::Keywords::desc_kw),
    std::make_pair("delete",    ECE141::Keywords::delete_kw),
    std::make_pair("describe",  ECE141::Keywords::describe_kw),
    std::make_pair("distinct",  ECE141::Keywords::distinct_kw),
    std::make_pair("double",    ECE141::Keywords::double_kw),
    std::make_pair("drop",      ECE141::Keywords::drop_kw),
     std::make_pair("dump",      ECE141::Keywords::dump_kw),
    std::make_pair("enum",      ECE141::Keywords::enum_kw),
    std::make_pair("explain",   ECE141::Keywords::explain_kw),
    std::make_pair("false",     ECE141::Keywords::false_kw),
    std::make_pair("float",     ECE141::Keywords::float_kw),
    std::make_pair("foreign",   ECE141::Keywords::foreign_kw),
    std::make_pair("from",      ECE141::Keywords::from_kw),
    std::make_pair("full",      ECE141::Keywords::full_kw),
    std::make_pair("group",     ECE141::Keywords::group_kw),
    std::make_pair("help",      ECE141::Keywords::help_kw),
    std::make_pair("in",        ECE141::Keywords::in_kw),
    std::make_pair("index",     ECE141::Keywords::index_kw),
    std::make_pair("indexes",   ECE141::Keywords::indexes_kw),
    std::make_pair("inner",     ECE141::Keywords::inner_kw),
    std::make_pair("insert",    ECE141::Keywords::insert_kw),
    std::make_pair("int",       ECE141::Keywords::integer_kw),
    std::make_pair("integer",   ECE141::Keywords::integer_kw),
    std::make_pair("into",      ECE141::Keywords::into_kw),
    std::make_pair("join",      ECE141::Keywords::join_kw),
    std::make_pair("key",       ECE141::Keywords::key_kw),
    std::make_pair("last",      ECE141::Keywords::last_kw),
    std::make_pair("left",      ECE141::Keywords::left_kw),
    std::make_pair("like",      ECE141::Keywords::like_kw),
    std::make_pair("limit",     ECE141::Keywords::limit_kw),
    std::make_pair("max",       ECE141::Keywords::max_kw),
    std::make_pair("min",       ECE141::Keywords::min_kw),
    std::make_pair("modify",    ECE141::Keywords::modify_kw),
    std::make_pair("not",       ECE141::Keywords::not_kw),
    std::make_pair("null",      ECE141::Keywords::null_kw),
    std::make_pair("on",        ECE141::Keywords::on_kw),
    std::make_pair("or",        ECE141::Keywords::or_kw),
    std::make_pair("order",     ECE141::Keywords::order_kw),
    std::make_pair("outer",     ECE141::Keywords::outer_kw),
    std::make_pair("primary",   ECE141::Keywords::primary_kw),
    std::make_pair("query",     ECE141::Keywords::query_kw),
    std::make_pair("quit",      ECE141::Keywords::quit_kw),
    std::make_pair("references", ECE141::Keywords::references_kw),
    std::make_pair("right",     ECE141::Keywords::right_kw),
    std::make_pair("rows",      ECE141::Keywords::rows_kw),
    std::make_pair("run",       ECE141::Keywords::run_kw),
    std::make_pair("select",    ECE141::Keywords::select_kw),
    std::make_pair("self",      ECE141::Keywords::self_kw),
    std::make_pair("set",       ECE141::Keywords::set_kw),
    std::make_pair("show",      ECE141::Keywords::show_kw),
    std::make_pair("sum",       ECE141::Keywords::sum_kw),
    std::make_pair("table",     ECE141::Keywords::table_kw),
    std::make_pair("tables",    ECE141::Keywords::tables_kw),
    std::make_pair("true",      ECE141::Keywords::true_kw),
    std::make_pair("unique",    ECE141::Keywords::unique_kw),
    std::make_pair("update",    ECE141::Keywords::update_kw),
    std::make_pair("use",       ECE141::Keywords::use_kw),
    std::make_pair("values",    ECE141::Keywords::values_kw),
    std::make_pair("varchar",   ECE141::Keywords::varchar_kw),
    std::make_pair("version",   ECE141::Keywords::version_kw),
    std::make_pair("where",     ECE141::Keywords::where_kw)
  };
  
  
  class Helpers {
  public:
    
    static Keywords getKeywordId(const std::string aKeyword) {
      auto theIter = gDictionary.find(aKeyword);
      if (theIter != gDictionary.end()) {
        return theIter->second;
      }
      return Keywords::unknown_kw;
    }
    
    //convert from char to keyword...
    static Keywords charToKeyword(char aChar) {
      switch(toupper(aChar)) {
        case 'I': return Keywords::integer_kw;
        case 'T': return Keywords::datetime_kw;
        case 'B': return Keywords::boolean_kw;
        case 'F': return Keywords::float_kw;
        case 'V': return Keywords::varchar_kw;
        default:  return Keywords::unknown_kw;
      }
    }
            
    static const char* dataTypeToString(DataTypes aType) {
      switch(aType) {
        case DataTypes::no_type:        return "none";
        case DataTypes::bool_type:      return "bool";
        case DataTypes::datetime_type:  return "datetime";
        case DataTypes::float_type:     return "float";
        case DataTypes::int_type:       return "int";
        case DataTypes::varchar_type:   return "varchar";
      }
    }
    
    static CommandType KWToCmd(Keywords aKWType) {
        return KWToCmdType.count(aKWType) ?
            KWToCmdType[aKWType] :
            CommandType::unknown_command;
    }

    static StatementType KWToStatement(Keywords aKWType) {
        return statements.count(aKWType) ?
            statements[aKWType] :
            StatementType::unknown;
    }

    static const char* keywordToString(Keywords aType) {
      switch(aType) {
        case Keywords::boolean_kw:    return "bool";
        case Keywords::create_kw:     return "create";
        case Keywords::database_kw:   return "database";
        case Keywords::databases_kw:  return "databases";
        case Keywords::datetime_kw:   return "datetime";
        case Keywords::describe_kw:   return "describe";
        case Keywords::drop_kw:       return "drop";
        case Keywords::float_kw:      return "float";
        case Keywords::integer_kw:    return "integer";
        case Keywords::show_kw:       return "show";
        case Keywords::table_kw:      return "table";
        case Keywords::tables_kw:     return "tables";
        case Keywords::use_kw:        return "use";
        case Keywords::varchar_kw:    return "varchar";
        default:                      return "unknown";
      }
    }
  
    // USE: ---validate that given keyword is a datatype...
    static bool isDatatype(Keywords aKeyword) {
      switch(aKeyword) {
        case Keywords::char_kw:
        case Keywords::datetime_kw:
        case Keywords::float_kw:
        case Keywords::integer_kw:
        case Keywords::varchar_kw:
          return true;
        default: return false;
      }
    }

    static bool isCmd(Keywords aKeyword) {
        switch (aKeyword) {
        case Keywords::about_kw:
        case Keywords::version_kw:
        case Keywords::help_kw:
        case Keywords::quit_kw:
            return true;
        default: return false;
        }
    }
        
    static Operators toOperator(std::string aString) {
      auto theIter = gOperators.find(aString);
      if (theIter != gOperators.end()) {
        return theIter->second;
      }
      return Operators::unknown_op;
    }
    
    static int getFunctionId(const std::string anIdentifier) {
      auto theIter = gFunctions.find(anIdentifier);
      if (theIter != gFunctions.end()) {
        return theIter->second;
      }
      return 0;
    }
    
    static bool isNumericKeyword(Keywords aKeyword) {
      static Keywords theTypes[]={Keywords::decimal_kw, Keywords::double_kw, Keywords::float_kw, Keywords::integer_kw};
      for(auto k : theTypes) {
        if(aKeyword==k) return true;
      }
      return false;
    }
 /*      
  static ECE141::Keywords gJoinTypes[]={
    ECE141::Keywords::cross_kw,ECE141::Keywords::full_kw, ECE141::Keywords::inner_kw,
    ECE141::Keywords::left_kw, ECE141::Keywords::right_kw
  };
*/
    
  };
  
  
}
#endif /* Helpers_h */
