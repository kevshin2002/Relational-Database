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
  using BlockVisitor = std::function<bool(const Block&, uint32_t)>;
  using BlockList = std::deque<uint32_t>;
  struct BlockIterator {
      virtual bool each(const BlockVisitor& aVisitor) = 0;
  };

  //--------------------------------------------------------------
  class Storage;
  class Storable {
  public:
      Storable(std::stringstream& aPayload) : contents(aPayload) {}
      virtual ~Storable() {}
      virtual StatusResult  encode(std::ostream& anOutput) = 0;

      virtual bool          save(std::fstream& aFile) = 0;
      virtual uint32_t&     getStorablePos() = 0;

      virtual size_t& getName() = 0;
      virtual StatusResult  decode(std::istream& anInput) = 0;
  protected:
      std::stringstream& contents;
  };
  using UniqueStorable = std::unique_ptr<Storable*>;
  class IndexStorable : public Block, public Storable {
  public:
      ~IndexStorable() {}
      StatusResult  encode(std::ostream& anOutput) override;
      bool          save(std::fstream& aFile) override;
      uint32_t&     getStorablePos() override;
      size_t& getName() override;
      StatusResult  decode(std::istream& anInput) override;

  protected:
      uint32_t nextIndex = 0;
  };
  

  class SchemaStorable : public Block, public Storable {
  public:
      SchemaStorable(BlockType aType, std::stringstream& aPayload, uint32_t aPointer, size_t aHash) : Block(aType, aPointer, aHash) , Storable(aPayload) {}
      ~SchemaStorable() {}
      StatusResult  encode(std::ostream& anOutput) override;
      bool          save(std::fstream& aFile) override;
      uint32_t&     getStorablePos() override;
      size_t& getName() override;
      StatusResult  decode(std::istream& anInput) override;
  };
  
/*  class DataStorable : public Block {
      StatusResult  encode(std::ostream& anOutput) override;
      StatusResult  decode(std::istream& anInput) override;
  };*/


  class Storage : public BlockIO, public BlockIterator {
  public:
        
    Storage(const std::string& aName, AccessMode aMode);
    ~Storage();

    // Create storables here (blocks)
    StatusResult add(StatementType aStmtType, DBQuery* aQuery);
    StatusResult drop(StatementType aStmtType, DBQuery* aQuery);

    std::vector<Table>& getTables() { return tables; }




    bool  each(const BlockVisitor& aVisitor) override;

    //What if we also offered a "storable" interface? 
    StatusResult save(const Storable &aStorable, int32_t aStartPos=kNewBlock);
    StatusResult load(std::iostream &aStream, uint32_t aStartBlockNum);

     //StatusResult markBlockAsFree(uint32_t aPos); maybe?
     //uint32_t     getFreeBlock(); //pos of next free (or new)...

  protected:
      std::stringstream makePayload(StatementType aType, DBQuery* aQuery);
      Storable* makeStorable(StatementType aType, std::stringstream& aPayload, size_t aHash);

      std::stringstream schemaPayload(DBQuery* aQuery);

      // API for creating specific blocks, is there a better way?
      // Maybe a StorageProcessor that creates a block type? Version 2.0 -> have AppController hold onto a StorageProcessor(which then holds onto a database)
      StatusResult    dataBlock(const StringList& aIdentifierList, const StringList& aValueList);
      StatusResult    indexBlock();
      StatusResult    schemaBlock(const Schema* aSchema);
      
      bool save();
      std::string loadContents(const AttributeList& anAttributeList);
      std::string loadContents(const StringList& aIdentifierList, const StringList& aValueList);

    uint32_t  getFreeBlock(); //pos of next free (or new)...

    bool   changed = false;  //might be helpful, or ignore if you prefer.
    std::stringstream buffer;

    Storable* storable;


    std::vector<Table> tables;
    std::vector<UniqueStorable> blocks;
    friend class Database;
  };

}


#endif /* Storage_hpp */
