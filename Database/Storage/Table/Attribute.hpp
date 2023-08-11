//
//  Attribute.hpp
//  PA3
//
//  Created by rick gessner on 4/18/22.
//  Copyright © 2023 rick gessner. All rights reserved.
//

#ifndef Attribute_hpp
#define Attribute_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <optional>
#include "../../../Misc/Types/keywords.hpp"

namespace ECE141 {
      
  class Attribute {
  public:
          
    Attribute(DataTypes aType=DataTypes::no_type);
    Attribute(std::string aName, DataTypes aType, uint32_t aSize=0);
    Attribute(const Attribute &aCopy);
    ~Attribute();
        
    const std::string&  getName() const {return name;}
    DataTypes           getType() const {return type;}
    
    //need more getters and setters right?
    Attribute& setSize(size_t aSize);

    Attribute& setDataType(DataTypes aType);
    Attribute& setPrimaryKey(bool aBool);
    Attribute& setAutoIncrement(bool aBool);
    Attribute& setNullable(bool aBool);


    bool                isValid(); //is this  valid?
  protected:
      std::string   name;
      DataTypes     type;

      size_t        size;
      bool          primary;
      bool          increment;
      bool          nullable;
      //what other properties do we need?
  };
  
  using AttributeOpt = std::optional<Attribute>;
  using AttributeList = std::vector<Attribute>;

}


#endif /* Attribute_hpp */