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

  class Storable {
  public:
      virtual ~Storable() {}
      Storable& setName(size_t ahashedString) {
          hashedName = ahashedString;
          return *this;
      }
      virtual StatusResult  encode(std::stringstream& anOutput) = 0;
      virtual bool          save(std::fstream& aFile) = 0;

      virtual StatusResult  decode(std::istream& anInput) = 0;
  protected:
      size_t hashedName;
  };
  using UniqueStorable = std::unique_ptr<Storable*>;
 /* class IndexStorable : public Block {
      StatusResult  encode(std::ostream& anOutput) const override;
      StatusResult  decode(std::istream& anInput) override;
  };
  */

  class SchemaStorable : public Block, public Storable {
  public:
      ~SchemaStorable() {}
      StatusResult  encode(std::stringstream& anOutput) override;
      bool          save(std::fstream& aFile) override;
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
    StatusResult add(BlockType aType, StatementType aStmtType, DBQuery* aQuery);
    StatusResult drop(BlockType aType, StatementType aStmtType, DBQuery* aQuery);

    bool  each(const BlockVisitor& aVisitor) override;

    //What if we also offered a "storable" interface? 
    StatusResult save(const Storable &aStorable, int32_t aStartPos=kNewBlock);
    StatusResult load(std::iostream &aStream, uint32_t aStartBlockNum);

     //StatusResult markBlockAsFree(uint32_t aPos); maybe?
     //uint32_t     getFreeBlock(); //pos of next free (or new)...

  protected:
      StatusResult    schemaBlock(const Schema* aSchema);
      StatusResult    dataBlock(const StringList& aIdentifierList, const StringList& aValueList);

      std::string loadContents(const AttributeList& anAttributeList);
      std::string loadContents(const StringList& aIdentifierList, const StringList& aValueList);

    uint32_t  getFreeBlock(); //pos of next free (or new)...

    std::vector<Table> tables;
    std::vector<UniqueStorable> blocks;
    friend class Database;
  };

}


#endif /* Storage_hpp */
