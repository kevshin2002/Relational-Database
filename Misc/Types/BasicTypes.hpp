//
//  BasicTypes.hpp
//  RGAssignement1
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#ifndef BasicTypes_h
#define BasicTypes_h

#include <optional>
#include <string>

namespace ECE141 {

  enum class DataTypes {
     no_type='N',  bool_type='B', datetime_type='D',
      float_type='F', int_type='I', varchar_type='V',
  };

  enum class CommandType {
      basic_command = 'B', db_command = 'D', sql_command = 'S', unknown_command = 'U'
  };

  using OptString = std::optional<std::string_view>;

}
#endif /* BasicTypes_h */
