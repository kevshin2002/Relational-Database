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
#include <deque>
#include <stack>
#include <set>
#include "../../../../Utilities/Tokenizer/Scanner.hpp"
#include "../../../../Utilities/Config.hpp"
#include "../../../../Utilities/Helpers.hpp"

namespace ECE141 {
  const int32_t kNewBlock = -1;
  const uint32_t Offset_One = 1;
  const size_t typeError = 14;
  const std::string kTableIndex = "tableIndex";

  class Storable;

  enum class BlockType {
    data_block='D',
    free_block='F',
    glossary_block='G',
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
      offset = aCopy.offset;
      return *this;
    }
   
    char          type;     //char version of block type
    uint32_t      name;
    uint32_t      offset = 0;
    //other properties? 
  };

  const size_t kBlockSize = 1024;
  const size_t kPayloadSize = (kBlockSize - sizeof(BlockHeader)) - 1;
  const uint32_t kFirstBlock = 0;
  //block .................
  class Block {
  public:
      Block(BlockType aType = BlockType::unknown_block, uint32_t aPointer = 1, std::string aName = "", uint32_t aHash = 0);
      Block(const Block& aCopy);
      Block& operator=(const Block& aCopy);

      uint32_t&  getHashName() { return header.name; }
      std::string   getPayload() { return payload; }
      bool          setName(std::string& aName) { name = aName; return true; }
      bool          setOffset(uint32_t anOffset) { header.offset = anOffset; return true; } // magic number, not sure 

      BlockHeader   header;
      char          payload[kPayloadSize];
      std::string   name;
  };

  using BlockVisitor = std::function<bool(uint32_t&, Block&)>;
  using BlockList = std::deque<uint32_t>;
  using Blocks = std::vector<Block>;
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
  using IndexID = std::pair<uint32_t, std::string>;
  using Indices = std::map<IndexID, char>;
  class BlockIO : public Scanner {
  public:
    BlockIO(const std::string& aName, AccessMode aMode);
    BlockIO(const BlockIO& aBlockIO);
    uint32_t&                            getBlockCount();
    uint32_t&                           getPointerIndex() { return pointerIndex; }
    Indices&   getIndices() { return indices; }
    
    bool    isBlockType(BlockType aType);
    virtual StatusResult  readBlock(uint32_t aBlockNumber, Block& aBlock);
    virtual StatusResult  writeBlock(uint32_t aBlockNumber, Block& aBlock);

  protected:

    Indices indices;

    std::fstream stream;
    uint32_t pointerIndex = 1;
  };

}


#endif /* BlockIO_hpp */
