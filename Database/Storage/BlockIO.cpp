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

  Block::Block(BlockType aType) {}

  Block::Block(const Block &aCopy) {
    *this=aCopy;
  }

  Block& Block::operator=(const Block &aCopy) {
    std::memcpy(payload, aCopy.payload, kPayloadSize);
    header=aCopy.header;
    return *this;
  }

  StatusResult Block::write(std::ostream &aStream) {
    return StatusResult{Errors::noError};
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
      stream.close();
      stream.open(aName.c_str(), theMode);
  }


  // USE: write data a given block (after seek) ---------------------------------------
  StatusResult BlockIO::writeBlock(uint32_t aBlockNum, Block &aBlock) {
    return StatusResult{Errors::writeError};
  }

  // USE: write data a given block (after seek) ---------------------------------------
  StatusResult BlockIO::readBlock(uint32_t aBlockNumber, Block &aBlock) {
    return StatusResult{Errors::readError};
  }

  // USE: count blocks in file ---------------------------------------
  uint32_t BlockIO::getBlockCount()  {
    return 0; //What should this be?
  }

}
