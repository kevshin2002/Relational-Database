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

  //Hmmm-- what could you do with this?....
  class Storable {
  public:
    virtual StatusResult  encode(std::ostream& anOutput) const=0;
    virtual StatusResult  decode(std::istream& anInput) =0;
    virtual bool          initHeader(Block& aBlock) const=0;    
  };
  using BlockVisitor = std::function<bool(const Block&, uint32_t)>;
  using BlockList = std::deque<uint32_t>;

  struct BlockIterator {
      virtual bool each(const BlockVisitor& aVisitor) = 0;
  };
  // USE: Our storage manager class...
  class Storage : public BlockIO, public BlockIterator {
  public:
        
    Storage(const std::string& aName, AccessMode aMode);
    ~Storage();
    StatusResult add(BlockType aType, StatementType aStmtType, DBQuery* aQuery);
    StatusResult drop(BlockType aType, StatementType aStmtType, DBQuery* aQuery);

    bool  each(const BlockVisitor& aVisitor) override;

    //What if we also offered a "storable" interface? 
    //StatusResult save(const Storable &aStorable, int32_t aStartPos=kNewBlock);
    //StatusResult load(std::iostream &aStream, uint32_t aStartBlockNum);

     //StatusResult markBlockAsFree(uint32_t aPos); maybe?
     //uint32_t     getFreeBlock(); //pos of next free (or new)...

  protected:
               
    uint32_t  getFreeBlock(); //pos of next free (or new)...
    
    friend class Database;
  };

}


#endif /* Storage_hpp */
