//
//  Row.hpp
//  PA3
//
//  Created by rick gessner on 4/2/23.
//

#ifndef Row_hpp
#define Row_hpp

#include <utility>
#include <variant>
#include <memory>
#include <map>
#include <vector>
#include "Attribute.hpp"
#include "Storable.hpp"

//feel free to use this, or create your own version...

namespace ECE141 {

  //These really shouldn't be here...
    using Value = std::variant<bool, int, float, std::string>;
  using KeyValues = std::map<const std::string, Value>;

  class Row {
  public:

    Row(uint32_t aID=0);
    Row(const Row &aRow);
    Row(Row&& aSource);
    
    ~Row();
    
    Row& operator=(const Row &aRow);
    bool operator==(const Row &aCopy) const;
    
      //STUDENT: What other methods do you require?
                          
    Row&                set(const std::string &aKey,
                            const Value &aValue);
        
    KeyValues&       getData() {return data;}
    
    //uint32_t            entityId; //hash value of entity?
    //uint32_t            blockNumber;

  protected:
    uint32_t         rowID;
    KeyValues        data;
  };

  //-------------------------------------------
  class RowCollection : public Storable {
  public:
      RowCollection(uint32_t aSchemaID = 0);
      RowCollection(const RowCollection& aCollection);
      ~RowCollection();

      RowCollection& operator=(const RowCollection& aCollection);

      std::istream& operator<<(std::istream& anInput) override;
      std::ostream& operator>>(std::ostream& anOutput) override;
      BlockHeader initHeader() override;
      

      bool add(Row& aRow);
      bool remove(Row& aRow);
      size_t size() { return rows.size(); }

  protected:
      uint32_t schemaID;
      std::vector<Row> rows;
  };


}
#endif /* Row_hpp */

