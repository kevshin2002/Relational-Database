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
#include "Table/Table.hpp"
#include "Table/BlockIO/Chunker.hpp"
#include "../../Statements/DBQuery.hpp"

namespace ECE141 {

  class Storage : public BlockIO, public BlockIterator {
  public:
        
    Storage(const std::string& aPath, AccessMode aMode);
    ~Storage();

    StatusResult        read(StatementType aStmtType, DBQuery* aQuery); // call load
    StatusResult        add(StatementType aStmtType, DBQuery* aQuery);
    StatusResult        drop(StatementType aStmtType, DBQuery* aQuery);

    bool each(const BlockVisitor& aVisitor) override;

    StatusResult        fetchTables(std::set<std::string>& aTableList);

    Schema*             getSchema(const std::string& aName);
    Table*              getTable(const std::string& aNacme);
    Tables&             getTables() { return tables; }
    
    
     //StatusResult markBlockAsFree(uint32_t aPos); maybe?
     

  protected:
     uint32_t&  getFreeBlock(size_t aCount);
     bool initialize();
     bool save();
      


   //pos of next free (or new)...

  private:
    bool   changed = false;  //might be helpful, or ignore if you prefer.

    BlockList freeBlocks;
    std::vector<Storable*> storables;

    //BlockIterator* iterator = nullptr;


    SchemaCollection schemas;
    RowCollection    rows;


    Tables tables; 
    friend class Database;
  };

}


#endif /* Storage_hpp */
