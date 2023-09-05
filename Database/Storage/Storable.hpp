
//
//  Storable.hpp
// 
//
//  Created by rick gessner on 2/27/23.
//

#ifndef Storable_hpp
#define Storable_hpp

#include "Table/BlockIO/BlockIO.hpp"

namespace ECE141 {
    class Storage;
    struct BlockIterator {
        virtual bool retrieve(std::fstream& aFile) = 0;
    };
    class Storable {
    public:
        Storable(std::stringstream& aPayload)
            : contents(aPayload) {}
        virtual ~Storable() {}
        virtual StatusResult  encode() = 0;
        virtual StatusResult  decode() = 0;

        virtual bool          save(std::fstream& aFile) = 0;
        virtual uint32_t&     getStorablePos() = 0;
        virtual BlockType     getType() = 0;
        virtual uint32_t&     getHash() = 0;
        virtual std::string&  getName() = 0;

    protected:
        std::stringstream& contents;
    };
    class IndexBlock : public Block, public Storable, public BlockIterator {
    public:
        IndexBlock(BlockType aType, std::stringstream& aPayload, uint32_t aPointer, uint32_t nextPointer, uint32_t aHash)
            : Block(aType, aPointer, kTableIndex, aHash), Storable(aPayload), nextIndex(nextPointer) {}
        IndexBlock(BlockType aType, std::stringstream& aPayload, uint32_t aPointer) : Block(aType, aPointer, kTableIndex, 0), Storable(aPayload), nextIndex(0) {}
        ~IndexBlock() {}

        bool          save(std::fstream& aFile) override;
        bool          retrieve(std::fstream& aFile) override;

        StatusResult  encode() override;
        StatusResult  decode() override;

        uint32_t&     getStorablePos() override;
        uint32_t&     getNextPos() { return nextIndex; }
        BlockType     getType() override;
        uint32_t&     getHash() override;
        std::string&  getName() override;

    protected:
        uint32_t nextIndex = 0;
    };

    class SchemaBlock : public Block, public Storable {
    public:
        SchemaBlock(BlockType aType, std::stringstream& aPayload, uint32_t aPointer, std::string& aName, uint32_t aHash)
            : Block(aType, aPointer, aName, aHash), Storable(aPayload) {}
        ~SchemaBlock() {}

        StatusResult  encode() override;
        StatusResult  decode() override;

        bool          save(std::fstream& aFile) override;
        uint32_t&     getStorablePos() override;
        BlockType     getType() override;
        uint32_t&     getHash() override;
        std::string&  getName() override;
    };

    class DataBlock : public Block, public Storable {
    public:
        DataBlock(BlockType aType, std::stringstream& aPayload, uint32_t aPointer, std::string& aName, uint32_t aHash)
            : Block(aType, aPointer, aName, aHash), Storable(aPayload) {}
        ~DataBlock() {}
        StatusResult  encode() override;
        StatusResult  decode() override;

        bool          save(std::fstream& aFile) override;
        uint32_t&     getStorablePos() override;
        BlockType     getType() override;
        uint32_t&     getHash() override;
        std::string&  getName() override;
    };
}

#endif // Storable.hpp