
//
//  Storable.hpp
// 
//
//  Created by rick gessner on 2/27/23.
//

#ifndef Storable_hpp
#define Storable_hpp

#include "BlockIO/BlockIO.hpp"

namespace ECE141 {
    class Storage;
    struct BlockIterator {
        virtual bool each(const BlockVisitor& aVisitor) = 0;
    };

    class Storable {
    public:
        Storable() {}
        virtual ~Storable() {}
        virtual std::istream& operator<<(std::istream& anInput) = 0;
        virtual std::ostream& operator>>(std::ostream& anOutput) = 0;
        virtual BlockHeader initHeader() = 0;
    };
}

#endif // Storable.hpp