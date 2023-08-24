//
//  BlockIO.hpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#ifndef BlockIO_hpp
#define BlockIO_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <variant>
#include <functional>
#include <cstring>
#include <string>
#include <deque>
#include <stack>
#include <optional>
#include <set>
#include "../../Misc/Types/Errors.hpp"
#include "../../Utilities/Helpers.hpp"


namespace ECE141 {
    class Storable;
  enum class BlockType {
    data_block='D',
    free_block='F',
    index_block='I',
    schema_block = 'S',
    unknown_block='U',
  };


  //a small header that describes the block...
  struct BlockHeader {
   
    BlockHeader(BlockType aType=BlockType::data_block, uint32_t aHash = 0)
      : type(static_cast<char>(aType)), name(aHash) {}

    BlockHeader(const BlockHeader& aCopy) {
      *this=aCopy;
    }
        
    void empty() {
      type=static_cast<char>(BlockType::free_block);
    }
    
    BlockHeader& operator=(const BlockHeader& aCopy) {
      type=aCopy.type;
      name = aCopy.name;
      return *this;
    }
   
    char          type;     //char version of block type
    uint32_t        name;
    //other properties? 
  };

  const size_t kBlockSize = 1024;
  const size_t kPayloadSize = kBlockSize - sizeof(BlockHeader) - 2;
  const uint32_t kFirstBlock = 0;
  //block .................
  using UniqueStorable = std::unique_ptr<Storable*>;
  class Block {
  public:
    Block(BlockType aType=BlockType::data_block, uint32_t aPointer=1, uint32_t aHash = 0);
    Block(const Block& aCopy);
    
    Block& operator=(const Block& aCopy);
   
    StatusResult write(std::ostream& aStream);
    bool         initHeader(BlockType aType, uint32_t hashedString);
    uint32_t&     getPos() { return position; }
    uint32_t&    getBlockName() { return header.name; }
    uint32_t      position;

    BlockHeader   header;
    char          payload[kPayloadSize];
  };

  //------------------------------

   struct CreateFile {
    operator std::ios_base::openmode() {
      return std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc;
    }
  }; //tags for db-open modes...
  struct OpenFile {
    operator std::ios_base::openmode() {
      return std::ios::binary | std::ios::in | std::ios::out;
    }
  };

  using AccessMode=std::variant<CreateFile, OpenFile>;

  class BlockIO {
  public:
    
    BlockIO(const std::string& aName, AccessMode aMode);
    uint32_t              chunk(std::string aContent);
    uint32_t              getBlockCount();
    virtual StatusResult  readBlock(uint32_t aBlockNumber, Block& aBlock);
    virtual StatusResult  writeBlock(uint32_t aBlockNumber, Block& aBlock);
    
  protected:
    std::map<uint32_t*, size_t> indices;
    std::deque<UniqueStorable> blocks;

    std::fstream stream;
    uint32_t pointerIndex = 1;
  };

}


#endif /* BlockIO_hpp */
