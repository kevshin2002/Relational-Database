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
		StringView() {}
		StringView(const char* aString) : view(aString) {}
		StringView(const std::string& aString) : view(aString) {}
		StringView& operator=(const char* aString) {
			view = aString;
			return *this;
		}
		StringView& operator=(const std::string& aString) {
			view = aString;
			return *this;
		}

		bool    show(std::ostream& aStream) override {
			aStream << view << " (" << Config::getTimer().elapsed() << " sec)\n";
			return true;
		}
	protected:
		std::string view;
	};

}

#endif // StringView.hpp