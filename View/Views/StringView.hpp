//
//  StringView.hpp
//  
//
//  Extension of Relational Database, ECE 141B
//  Copyright UCSD ECE 141B, Prof Rick Gessner
//  Coded by Kevin Shin
//


#ifndef StringView_hpp
#define StringView_hpp

#include "View.hpp"

namespace ECE141 {
	class StringView : public View{
	public:
		StringView(std::string &aString) : view(aString) {}

		bool    show(std::ostream& aStream) override {
			aStream << view;
			return true;
		}
	protected:
		std::string view;
	};

}

#endif // StringView.hpp