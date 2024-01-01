//
//  Index.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef Index_hpp
#define Index_hpp

#include "Table/Storable.hpp"

namespace ECE141 {
	//Separate later on once complexity rises.
	class Index : public Storable {
	public:
		Index(Indices& anIndices) : indices(anIndices) {}
		std::istream& operator<<(std::istream& anInput) override {
			return anInput;
		}
		std::ostream& operator>>(std::ostream& anOutput) override {
			for (const auto& thePair : indices) {
				anOutput << thePair.first.second << " " << thePair.second << " " << thePair.first.first << " \\ ";
			}
			return anOutput;
		}
		BlockHeader initHeader() override {return BlockHeader(BlockType::glossary_block, kFirstBlock); }
		std::string getName() override { return kTableIndex; }
		uint32_t getSize() override { return kPayloadSize; } // magic num

	protected:
		Indices& indices;
	};

	class IndicesIndex : public Index {
	public:
		std::istream& operator<<(std::istream& anInput) override;
		std::ostream& operator>>(std::ostream& anOutput) override;
		BlockHeader initHeader() override;
	};

	class SchemaIndex : public Index {
	public:
		std::istream& operator<<(std::istream& anInput) override;
		std::ostream& operator>>(std::ostream& anOutput) override;
		BlockHeader initHeader() override;
	};

	class DataIndex : public Index {
	public:
		std::istream& operator<<(std::istream& anInput) override;
		std::ostream& operator>>(std::ostream& anOutput) override;
		BlockHeader initHeader() override;
	};

}

#endif // Index.hpp