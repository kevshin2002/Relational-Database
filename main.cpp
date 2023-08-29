//
//  main.cpp
//  Database2
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

// visual studio code memory leak detector
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "Tests/Testing/TestManually.hpp"
#include "Tests/Testing/TestAutomatic.hpp"

//----------------------------------------------

int main(int argc, const char* argv[]) {
    //_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
    srand(static_cast<uint32_t>(time(0)));

    if (argc > 1) {

        ECE141::TestAutomatic run;
        static ECE141::TestCalls theCalls{
          {"about", [&](std::stringstream& anOut) {
            return run.aboutTest(anOut); }},
          {"cache", [&](std::stringstream& anOut) {
            return run.cacheCommandTest(anOut); }},
          {"compile", [&](std::stringstream& anOut) {
            return run.compileTest(anOut); }},
          {"delete", [&](std::stringstream& anOut) {
            return run.deleteCommandTest(anOut); }},
          {"db", [&](std::stringstream& anOut) {
            return run.dbCommandsTest(anOut); }},
          {"drop", [&](std::stringstream& anOut) {
            return run.dropCommandTest(anOut); }},
          {"dojo",    [&](std::stringstream& anOut) {
            return run.dojoTest(anOut); }},
          {"filter", [&](std::stringstream& anOut) {
            return run.filterCommandTest(anOut); }},
          {"help",   [&](std::stringstream& anOut) {
            return run.helpTest(anOut); }},
          {"index", [&](std::stringstream& anOut) {
            return run.indexCommandTest(anOut); }},
          {"insert", [&](std::stringstream& anOut) {
            return run.insertCommandTest(anOut); }},
          {"join", [&](std::stringstream& anOut) {
            return run.joinCommandTest(anOut); }},
          {"mutate", [&](std::stringstream& anOut) {
            return run.mutateCommandTest(anOut); }},
          {"overfill", [&](std::stringstream& anOut) {
            return run.insertCommandTest(anOut,20); }},
          {"select", [&](std::stringstream& anOut) {
            return run.selectCommandTest(anOut); }},
          {"table", [&](std::stringstream& anOut) {
            return run.tableCommandsTest(anOut); }},
          {"parse", [&](std::stringstream& anOut) {
            return run.parseTest(anOut); }},
          {"update", [&](std::stringstream& anOut) {
            return run.updateCommandTest(anOut); }},
          {"quit", [&](std::stringstream& anOut) {
            return run.quitTest(anOut); }},
          {"version", [&](std::stringstream& anOut) {
            return run.versionTest(anOut); }},
        };
        int* myInt = new int;
        *myInt = 42;

        std::string theCmd(argv[1]);
        std::transform(theCmd.begin(), theCmd.end(), theCmd.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (theCalls.count(theCmd)) {
            std::stringstream theOutput;
            bool theResult = theCalls[theCmd](theOutput);
            const char* theStatus[] = { "FAIL","PASS" };
            std::cout << theCmd << " test " << theStatus[theResult] << "\n";
            std::cout << "---------------------------------\n" << theOutput.str() << "\n";
        }
        else std::cout << "Unknown test\n";
    }
    else {
        doManualTesting();
    }
    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    //_CrtDumpMemoryLeaks();
    return 0;
}
