//
//  TestSequencer.hpp
//  Database
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef TestSequencer_h
#define TestSequencer_h

#include <initializer_list>
#include "../../Utilities/Tokenizer/Tokenizer.hpp"

namespace ECE141 {


    class TestSequencer {
    public:
        TestSequencer(Tokenizer& aTokenizer)
            : tokenizer(aTokenizer), state{ true } {}


        TestSequencer& nextIs(const KWList& aList, bool autoSkip = false) {
            if (state) {
                std::vector<Keywords> theList{ aList };
                int thePos{ 0 };
                for (auto theKW : theList) {
                    Token& theToken = tokenizer.peek(thePos++);
                    if (theKW != theToken.keyword) {
                        state = false;
                        break;
                    }
                }
                if (state && autoSkip) { return skip(theList.size()); }
            }
            return *this;
        }

        TestSequencer& getIdentifier(std::string& aName) {
            return *this;
        }

        TestSequencer& getNumber(int& aValue) {
            Token& theToken = tokenizer.current();
            aValue = std::stoi(theToken.data);
            return *this;
        }

        TestSequencer& skipPast(Keywords aKW) {
            if (state) {
                while (tokenizer.next()) {
                    auto theToken = tokenizer.current();
                    if (theToken.keyword == aKW) {
                        tokenizer.next();//skip it...
                        return *this;
                    }
                }
                state = false;
            }
            return *this;
        }

        TestSequencer& skipPast(char aChar) {
            if (state) {
                while (tokenizer.next()) {
                    auto theToken = tokenizer.current();
                    if (theToken.type == TokenType::punctuation) {
                        if (theToken.data[0] == aChar) {
                            tokenizer.next();//skip it...
                            return *this;
                        }
                    }
                }
                state = false;
            }
            return *this;
        }

        TestSequencer& skip(size_t aCount) {
            if (state) {
                tokenizer.next((int)aCount); //actually skip...
            }
            return *this;
        }

        operator bool() { return state; }

        TestSequencer& clear() {
            state = true;
            return *this;
        }

    protected:
        Tokenizer& tokenizer;
        bool      state;
    };

}

#endif /* TestSequencer_h */
