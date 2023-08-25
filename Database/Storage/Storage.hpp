//
//  Storage.hpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#ifndef Storage_hpp
#define Storage_hpp

#include "../../Database/Storage/Table/BlockIO/BlockIO.hpp"
#include "../../Database/Storage/Table/Table.hpp"
#include "../../Statements/DBQuery.hpp"

namespace ECE141 {
  const int32_t kNewBlock=-1;
  const size_t typeError = 14;
  const std::string kTableIndex = "tableIndex";
  using BlockVisitor = std::function<bool(const Block&, uint32_t)>;
  using BlockList = std::deque<uint32_t>;

  /*
  
  */
  struct BlockIterator {
      virtual std::deque<Block>& getBlocks() { return loaded_blocks; }
      virtual bool retrieve(const BlockVisitor& aVisitor) = 0;

      std::deque<Block> loaded_blocks;
      
  };

  //--------------------------------------------------------------
  class Storage;
  class Storable {
  public:
      Storable(std::stringstream& aPayload)
      : contents(aPayload) {}
      virtual ~Storable() {}
      virtual StatusResult  encode() = 0;
      virtual StatusResult  decode(std::istream& anInput) = 0;

      virtual bool          save(std::fstream& aFile) = 0;
      virtual uint32_t&     getStorablePos() = 0;
      virtual BlockType     getType()  =0;
      virtual uint32_t&     getHash() = 0;
      virtual std::string&  getName() = 0;

  protected:
      std::stringstream& contents;
  };

  class IndexStorable : public Block, public Storable {
  public:
      IndexStorable(BlockType aType, std::stringstream& aPayload, uint32_t aPointer, uint32_t nextPointer, uint32_t aHash)
      : Block(aType, aPointer, kTableIndex, aHash), Storable(aPayload), nextIndex(nextPointer) {}
      ~IndexStorable() {}

      StatusResult  encode() override;
      StatusResult  decode(std::istream& anInput) override;

      bool          save(std::fstream& aFile) override;
      uint32_t&     getStorablePos() override;
      BlockType     getType() override;
      uint32_t&     getHash() override;
      std::string&  getName() override;

  protected:
      uint32_t nextIndex = 0;
  };
  

  class SchemaStorable : public Block, public Storable {
  public:
      SchemaStorable(BlockType aType, std::stringstream& aPayload, uint32_t aPointer, std::string& aName, uint32_t aHash)
      : Block(aType, aPointer, aName, aHash) , Storable(aPayload) {}
      ~SchemaStorable() {}

      StatusResult  encode() override;
      StatusResult  decode(std::istream& anInput) override;

      bool          save(std::fstream& aFile) override;
      uint32_t&     getStorablePos() override;
      BlockType     getType() override;
      uint32_t&     getHash() override;
      std::string&  getName() override;
  };

  class DataStorable : public Block, public Storable {
  public:
      DataStorable(BlockType aType, std::stringstream& aPayload, uint32_t aPointer, std::string& aName, uint32_t aHash)
      : Block(aType, aPointer, aName, aHash), Storable(aPayload) {}
      ~DataStorable() {}
      StatusResult  encode() override;
      StatusResult  decode(std::istream& anInput) override;

      bool          save(std::fstream& aFile) override;
      uint32_t&     getStorablePos() override;
      BlockType     getType() override;
      uint32_t&     getHash() override;
      std::string&  getName() override;
  };

  class IndexIterator : public BlockIterator {

  };

  class Storage : public BlockIO, public BlockIterator{
  public:
        
    Storage(const std::string& aPath, AccessMode aMode);
    ~Storage();

    // Create storables here (blocks)
    StatusResult read(StatementType aStmtType, DBQuery* aQuery);
    StatusResult add(StatementType aStmtType, DBQuery* aQuery);
    StatusResult drop(StatementType aStmtType, DBQuery* aQuery);
    bool retrieve(const BlockVisitor& aVisitor) override;

    StatusResult fetchTables(std::set<std::string>& aTableList);
    std::vector<Table>& getTables() { return tables; }
    Table* getTable(const std::string& aNacme);
    Schema* getSchema(const std::string& aName);
    

    //What if we also offered a "storable" interface? 
    StatusResult save(const Storable &aStorable, int32_t aStartPos=kNewBlock);
    StatusResult load(std::iostream &aStream, uint32_t aStartBlockNum);

     //StatusResult markBlockAsFree(uint32_t aPos); maybe?
     //uint32_t     getFreeBlock(); //pos of next free (or new)...

  protected:
      std::stringstream makePayload(StatementType aType, DBQuery* aQuery);
      Storable*         makeStorable(StatementType aType, std::stringstream& aPayload, std::string aName, uint32_t aHash);

      std::stringstream schemaPayload(DBQuery* aQuery);
      std::stringstream dataPayload(DBQuery* aQuery);

      // Maybe a StorageProcessor that creates a block type? Version 2.0 -> have AppController hold onto a StorageProcessor(which then holds onto a database)
      StatusResult    indexBlock();
      bool load_meta();
      bool save();



      // not needed but here cause there's just so much going on in the low level and idk which to delete
      std::string loadContents(const AttributeList& anAttributeList);
      std::string loadContents(const StringList& aIdentifierList, const StringList& aValueList);

    uint32_t  getFreeBlock(); //pos of next free (or new)...

    bool   changed = false;  //might be helpful, or ignore if you prefer.
    std::stringstream buffer;

    Storable* storable;

    SchemaCollection schemas;
    RowCollection    rows;

    std::vector<Table> tables; 
    friend class Database;
  };

}


#endif /* Storage_hpp */
