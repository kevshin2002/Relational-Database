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
#include "BufferManager.hpp"

namespace ECE141 {

  class Storage : public BlockIO {
  public:
        
    Storage(const std::string& aPath, AccessMode aMode);
    ~Storage();

    StatusResult        read(StatementType aStmtType, DBQuery* aQuery); // call load

    StatusResult        add(StatementType aStmtType, DBQuery* aQuery);
    StatusResult        drop(StatementType aStmtType, DBQuery* aQuery);

    StatusResult        fetchTables(std::set<std::string>& aTableList);

    Schema*             getSchema(const std::string& aName);
    Table*              getTable(const std::string& aNacme);
    Tables& getTables() { return tables; }
    
    
     //StatusResult markBlockAsFree(uint32_t aPos); maybe?
     //uint32_t     getFreeBlock(); //pos of next free (or new)...

  protected:

      
      // Maybe a StorageProcessor that creates a block type? Version 2.0 -> have AppController hold onto a StorageProcessor(which then holds onto a database)
      StatusResult    indexBlock();

      bool load(BlockIterator& anIterator);
      bool initialize();
      bool save();



    uint32_t  getFreeBlock(); //pos of next free (or new)...

  private:
    bool   changed = false;  //might be helpful, or ignore if you prefer.
    BufferManager buffer;

    //BlockIterator* iterator = nullptr;


    SchemaCollection schemas;
    RowCollection    rows;


    Tables tables; 
    friend class Database;
  };

}


#endif /* Storage_hpp */
