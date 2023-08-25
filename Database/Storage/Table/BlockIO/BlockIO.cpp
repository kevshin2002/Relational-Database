//
//  BlockIO.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#include <cstring>
#include "BlockIO.hpp"
#include "../../Utilities/Config.hpp"

namespace ECE141 {

    Block::Block(BlockType aType, uint32_t aPointer, std::string aName, uint32_t aHash) : position(aPointer), name(aName) {
        header = BlockHeader(aType, aHash);
    }

  Block::Block(const Block &aCopy) {
    *this=aCopy;
  }

  Block& Block::operator=(const Block &aCopy) {
    std::memcpy(payload, aCopy.payload, kPayloadSize);
    header=aCopy.header;
    return *this;
  }

  StatusResult Block::write(std::ostream &aStream) {
      aStream.seekp(position * kBlockSize);
      aStream.write(payload, kBlockSize);
      aStream.flush();
    return StatusResult{Errors::noError};
  }

  bool Block::initHeader(BlockType aType, uint32_t hashedString) {
      header.name = hashedString;
      return true;
  }
  //---------------------------------------------------

  struct modeToInt {
      std::ios_base::openmode operator()(CreateFile& aVal) const { return static_cast<std::ios_base::openmode>(aVal); }
      std::ios_base::openmode operator()(OpenFile& aVal) const { return static_cast<std::ios_base::openmode>(aVal); }
  };

  BlockIO::BlockIO(const std::string& aName, AccessMode aMode) {

      auto  theMode = std::visit(modeToInt(), aMode);
      stream.clear(); // Clear flag just-in-case...
      stream.open(aName.c_str(), theMode); //force truncate if...
  }

  // USE: write data a given block (after seek) ---------------------------------------
  StatusResult BlockIO::writeBlock(uint32_t aBlockNum, Block &aBlock) {
    return StatusResult{Errors::writeError};
  }

  StatusResult BlockIO::readIndex(uint32_t aBlockNumber, Storable* aStorable) {
      std::stringstream theContents;
      char theBuffer[kBlockSize];
      stream.read(theBuffer, aBlockNumber * kBlockSize);
      theContents << theBuffer;
      
      char theType;
      theContents >> theType;
    //  aStorable. // = theType == static_cast<char>(BlockType::index_block) ? theType : static_cast<char>(BlockType::unknown_block);
      return Errors::notImplemented;
  }
  // USE: write data a given block (after seek) ---------------------------------------
  StatusResult BlockIO::readBlock(uint32_t aBlockNumber, Block &aBlock) {
    std::stringstream theContents;
    char theBuffer[kBlockSize];
    stream.read(theBuffer, aBlockNumber * kBlockSize);
    theContents << theBuffer;

    
    return StatusResult{Errors::readError};
  }

  // USE: count blocks in file ---------------------------------------
  uint32_t BlockIO::getBlockCount()  {
     stream.seekg(0, std::ios::end); 
     std::streampos fileSize = stream.tellg(); 
     stream.seekg(0, std::ios::beg); 
     return static_cast<uint32_t>(fileSize / kBlockSize);
  }

  uint32_t BlockIO::chunk(std::string aContent) {
      uint32_t theNum = std::ceil(aContent.size() / kPayloadSize);
      return theNum == 0 ? 1 : theNum;
  }
}
