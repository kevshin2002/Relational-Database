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
    //other types?
    schema_block = 'S',
    unknown_block='U',
  };


  //a small header that describes the block...
  struct BlockHeader {
   
    BlockHeader(BlockType aType=BlockType::data_block)
      : type(static_cast<char>(aType)) {}

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
    size_t        name;
    //other properties?
  };

  const size_t kBlockSize = 1024;
  const size_t kPayloadSize = kBlockSize - sizeof(BlockHeader);
  
  //block .................
  class Block {
  public:
    Block(BlockType aType=BlockType::data_block);
    Block(const Block& aCopy);
    
    Block& operator=(const Block& aCopy);
   
    StatusResult write(std::ostream& aStream);
    bool         initHeader(BlockType aType, size_t hashedString);

    BlockHeader   header;
    char          payload[kPayloadSize];
    uint32_t      position;
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
    std::fstream stream;
    uint32_t pointerIndex = 0;
  };

}


#endif /* BlockIO_hpp */
