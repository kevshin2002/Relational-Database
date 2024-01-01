//
//  BlockIO.cpp
//  PA2
//
//  Created by rick gessner on 2/27/23.
//

#include "BlockIO.hpp"

namespace ECE141 {

    Block::Block(BlockType aType, uint32_t aPointer, std::string aName, uint32_t aHash) {
        header = BlockHeader(aType, aHash);
    }

  Block::Block(const Block &aCopy) {
    *this=aCopy;
  }

  Block& Block::operator=(const Block &aCopy) {
    std::memcpy(payload, aCopy.payload, kPayloadSize);
    header=aCopy.header;
    name = aCopy.name;
    return *this;
  }


  //---------------------------------------------------

  struct modeToInt {
      std::ios_base::openmode operator()(CreateFile& aVal) const { return static_cast<std::ios_base::openmode>(aVal); }
      std::ios_base::openmode operator()(OpenFile& aVal) const { return static_cast<std::ios_base::openmode>(aVal); }
  };

  BlockIO::BlockIO(const std::string& aName, AccessMode aMode) : Scanner(stream) {
      auto  theMode = std::visit(modeToInt(), aMode);
      stream.clear(); 
      stream.open(aName.c_str(), theMode); 
  }

  BlockIO::BlockIO(const BlockIO& aBlockIO) : Scanner(stream) {
      indices = aBlockIO.indices;
      pointerIndex = aBlockIO.pointerIndex;
  }
  // Rework
  StatusResult BlockIO::writeBlock(uint32_t aBlockNum, Block& aBlock) {
      std::stringstream theStream;
      if (aBlock.header.type != static_cast<char>(BlockType::free_block) && static_cast<char>(BlockType::unknown_block)) {
          theStream << aBlock.header.type << " ";
          if (aBlock.header.name == kFirstBlock)
              theStream << std::setw(9) << std::setfill('0') << " ";
          else
              theStream << aBlock.header.name << " ";
          theStream << aBlock.header.offset << " ";
          theStream << aBlock.payload;
      }
      else {
          theStream << "  ";
          theStream << std::setw(9) << std::setfill(' ') << " ";
          theStream << "  ";

          std::memset(aBlock.payload, ' ', kPayloadSize);
          theStream << aBlock.payload;
      }
      stream.seekp(aBlockNum * kBlockSize);
      stream.write(theStream.str().c_str(), kBlockSize);
      stream.flush();
      return Errors::noError;
  }

  StatusResult BlockIO::readBlock(uint32_t aBlockNum, Block& aBlock) {
      char theBuffer[kBlockSize];

      stream.seekg(aBlockNum * kBlockSize);
      stream.read(theBuffer, kBlockSize);

      std::stringstream theStream(theBuffer);
      theStream >> aBlock.header.type
          >> aBlock.name
          >> aBlock.header.offset;

      theStream.read(aBlock.payload, kPayloadSize);
      return Errors::noError;
  }


  // USE: count blocks in file ---------------------------------------
  uint32_t& BlockIO::getBlockCount()  {
     stream.seekg(0, std::ios::end); 
     std::streampos fileSize = stream.tellg(); 
     stream.seekg(0, std::ios::beg); 
     uint32_t theSize = static_cast<uint32_t>(fileSize / kBlockSize);
     pointerIndex = theSize == 0 ? 1 : theSize;
     return pointerIndex;
  }

  bool BlockIO::isBlockType(BlockType aType) {
      switch (aType) {
      case (BlockType::data_block):
      case (BlockType::free_block):
      case (BlockType::glossary_block):
      case (BlockType::index_block):
      case (BlockType::schema_block):
          return true;
      case (BlockType::unknown_block):
      default:
          return false;
      }
  }
}
