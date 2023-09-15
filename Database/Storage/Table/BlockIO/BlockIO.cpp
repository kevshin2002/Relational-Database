//
//  BlockIO.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#include "BlockIO.hpp"

namespace ECE141 {

    Block::Block(BlockType aType, uint32_t aPointer, std::string aName, uint32_t aHash) : name(aName) {
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

  StatusResult BlockIO::writeBlock(uint32_t aBlockNum, Block& aBlock) {
      std::stringstream theStream;
      theStream << aBlock.header.type << " " << aBlock.header.name << " ";
      theStream << aBlock.payload;
      stream.seekp(aBlockNum * kBlockSize);
      stream.write(theStream.str().c_str(), kBlockSize);
      stream.flush();
      return Errors::noError;
  }

  StatusResult BlockIO::readBlock(uint32_t aBlockNumber, Block& aBlock) {
      return StatusResult{ Errors::readError };
  }

  // USE: count blocks in file ---------------------------------------
  uint32_t BlockIO::getBlockCount()  {
     stream.seekg(0, std::ios::end); 
     std::streampos fileSize = stream.tellg(); 
     stream.seekg(0, std::ios::beg); 
     pointerIndex = static_cast<uint32_t>(fileSize / kBlockSize);
     return pointerIndex;
  }

}
