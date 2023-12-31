//
//  Schema.hpp
//  PA3
//
//  Created by rick gessner on 3/18/23.
//  Copyright © 2023 rick gessner. All rights reserved.
//

#ifndef Schema_hpp
#define Schema_hpp

#include "Row.hpp"

namespace ECE141 {
  class Database;
  class Schema : public Storable {
  public:
    Schema(const std::string& aName);
    Schema(Database* aDB);
    Schema(const Schema &aCopy);
    ~Schema();
    std::istream& operator<<(std::istream& anInput) override;
    std::ostream& operator>>(std::ostream& anOutput) override;
    BlockHeader initHeader() override;
    std::string getName() override { return name; }
    uint32_t    getSize() override { return kPayloadSize; } // fix
    Schema&               setName(const std::string& aName); 
    Schema&               addAttribute(const Attribute& anAttribute);
    Schema&               setAttribute(Attribute& aAttribute, const std::string& aString);
    const std::string&    getName() const {return name;}
    Attribute*            getAttribute(const std::string& aField);
    AttributeList&        getAttributes() { return attributes; }
    Database*             getDatabase() { return database; }
    uint32_t              getHash() const { return hashedName; }
    
 
    bool isEmpty() { return attributes.size(); }
  protected:
    Database*     database;
    AttributeList   attributes;
    uint32_t          hashedName;
    std::string     name;
    
    //how will you manage creation of primary keys?
    
  };
  using SchemaCollection = std::map<std::string, Schema>;
  
}
#endif /* Schema_hpp */
