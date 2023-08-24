//
//  Table.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//

#ifndef Table_hpp
#define Table_hpp

#include "Schema.hpp"

namespace ECE141 {
	class Table {
	public:
		Table(const Schema* aSchema) : schema(*aSchema) {}
		Schema& getSchema() { return schema; }
	protected:
		Schema schema;
	};
	using Tables = std::deque<Table>;
	using TableOpt = std::optional<Table>;
}

#endif // Table.hpp