//
//  BufferManager.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef BufferManager_hpp
#define BufferManager_hpp

#include "Storable.hpp"
#include "../../Database/Storage/Table/Table.hpp"
#include "../../Statements/DBQuery.hpp"

namespace ECE141 {
	class BufferManager {
	public:
		BufferManager(Tables& aTableList) : tables(aTableList) {}
		~BufferManager() {}
		StatusResult	  makePayload(StatementType aType, DBQuery* aQuery);
		Storable*         makeStorable(StatementType aType, uint32_t& aPosition, std::string aName, uint32_t aHash);

		std::stringstream& getInput() { return inputBuffer; }
		std::stringstream& getOutput() { return outputBuffer; }

	protected:
		std::stringstream schemaPayload(DBQuery* aQuery);
		std::stringstream dataPayload(DBQuery* aQuery);

	private:
		std::stringstream inputBuffer;
		std::stringstream outputBuffer;

		Tables& tables;
		Storable* storable;
	};
}

#endif // BufferManager.hpp