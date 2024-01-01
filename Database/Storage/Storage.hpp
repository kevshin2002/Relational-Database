//
//  Storage.hpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#ifndef Storage_hpp
#define Storage_hpp

#include <cmath>
#include <cstdlib>
#include <bitset>
#include "Index.hpp"
#include "Table/BlockIO/Chunker.hpp"
#include "../../Statements/DBQuery.hpp"
#include "Table/Schema.hpp"

namespace ECE141 {

  class Storage : public BlockIO, public BlockIterator {
  public:
        
    Storage(const std::string& aPath, AccessMode aMode);
    Storage(const Storage& aStorage);
    ~Storage();

    StatusResult        read(StatementType aStmtType, DBQuery* aQuery); // call load
    StatusResult        add(StatementType aStmtType, DBQuery* aQuery);
    StatusResult        drop(const std::string& aName, DBQuery* aQuery);             
    bool each(const BlockVisitor& aVisitor) override;


    Schema*             getSchema(const std::string& aName);
    RowCollection& getRows(const std::string& aName) { return rows; }
    
     //StatusResult markBlockAsFree(uint32_t aPos); maybe?
     

  protected:
     StatusResult        fetchTables(std::set<std::string>& aTableList);
     uint32_t&           getFreeBlock(size_t aCount);
     StatusResult        getData(std::string aName);
     bool initialize();
     bool save();
     bool index(bool aConfig);
      


   //pos of next free (or new)...

  private:
    bool   changed = false;  //might be helpful, or ignore if you prefer.

    BlockList freeBlocks;
    std::vector<Storable*> storables;

    //BlockIterator* iterator = nullptr;


    SchemaCollection schemas;
    RowCollection    rows;


    friend class Database;
  };

}


#endif /* Storage_hpp */
