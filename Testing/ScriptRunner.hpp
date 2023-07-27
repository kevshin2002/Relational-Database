//
//  ScriptRunner.hpp
//  Database
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef ScriptRunner_h
#define ScriptRunner_h

#include <fstream>
#include <sstream>
#include "../Controller/AppController.hpp"

namespace ECE141 {

    class ScriptRunner {
    public:
        ScriptRunner(AppController& anApp) : app(anApp) {}

        std::string readCommand(std::istream& anInput) {
            std::string theResult;
            char theChar;
            while (!anInput.eof()) {
                anInput >> theChar;
                if (semicolon == theChar || 0 == theChar) break;
                else theResult += theChar;
            }

            static const char* typeOfWhitespaces = " \t\n\r\f\v";
            theResult.erase(theResult.find_last_not_of(typeOfWhitespaces) + 1);
            theResult.erase(0, theResult.find_first_not_of(typeOfWhitespaces));

            return theResult;
        }

        StatusResult run(std::istream& anInput, std::ostream& anOutput) {
            StatusResult theResult;

            auto theTimer = Config::getTimer();
            auto theStart = theTimer.now();

            while (theResult) {
                std::string theCommand(readCommand(anInput));

                if (theCommand.length()) {
                    std::stringstream theStream(theCommand);
                    anOutput << theCommand << "\n";
                    theResult = app.handleInput(theStream, [&](View& aView) {
                        aView.show(anOutput);
                    anOutput << std::endl;
                        });
                }
                else break;
            }
            anOutput << "Elapsed: " << std::fixed
                << theTimer.elapsed(theStart) << "\n";
            if (Errors::userTerminated == theResult.error) {
                theResult = Errors::noError;
            }
            return theResult;
        }

    protected:
        AppController& app;
    };

}

#endif /* ScriptRunner_h */
