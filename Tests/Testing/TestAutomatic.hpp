//
//  TestAutomatic.hpp
//  Database
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef TestAutomatic_h
#define TestAutomatic_h

#include <random>
#include <stack>
#include "TestSequencer.hpp"
#include "../Scripts/ScriptRunner.hpp"
#include "../../Utilities/FolderReader.hpp"
#include "../Test Utilities/Faked.hpp"

using namespace ECE141;
namespace fs = std::filesystem;

namespace ECE141 {

    using Validate = std::function<bool(const std::string& aBuffer)>;
    using TestCall = std::function<bool(std::stringstream& anOutput)>;
    using InputLine = std::pair<std::string_view, size_t>;
    using LineList = std::initializer_list<InputLine>;
    using InitList = std::initializer_list<std::string>;
    using TestCalls = std::map<std::string, TestCall>;

    const std::string_view theVersion{ "Tester(1.7)" };
    const size_t kMaxErrors{ 100 };

    using CountList = std::vector<int>;

    //------------------------------------

    enum Commands {
        unknown, alter, createDB, createTable, describe,
        delet, dropDB, dropTable, dumpDB, error, insert, select, showDBs,
        showTables, showIndex, showIndexes, update, useDB
    };

    struct CommandCount {
        Commands  command;
        int       count;
        char      cmp = '=';
        bool      error = false;

        bool compare(const CommandCount& anItem) {
            if (command == anItem.command) {
                if (anItem.error != error) return false;
                return ('>' == cmp) ? anItem.count > count : anItem.count == count;
            }
            return false;
        }
    };

    using Responses = std::vector<CommandCount>;
    using ExpectList = std::initializer_list<CommandCount>;

    class Expected {
    public:
        Expected(const ExpectList& aList) : expected(aList) {}

        bool operator==(const Responses& aResponses) {
            size_t theSize = expected.size();
            if (theSize && aResponses.size() == theSize) {
                for (size_t i = 0; i < theSize; i++) {
                    if (!expected[i].compare(aResponses[i])) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        Expected& add(const CommandCount& aCommand) {
            expected.push_back(aCommand);
            return *this;
        }

    protected:
        Responses expected;
    };

    //------------------------------------

    class TestAutomatic {
    public:

        ~TestAutomatic() {
            std::cout << "---------- " << theVersion << " ----------\n";
        }

        bool compileTest(std::ostream& anOutput) { return true; }

        //----------------------------------------------

        bool matches(const std::string& aBuffer, const LineList& aLines) {
            std::stringstream theInput(aBuffer);
            std::string theLine;
            for (auto& theItem : aLines) {
                if (std::getline(theInput, theLine)) {
                    if (theLine.find(theItem.first) != std::string::npos) {
                        if (theLine.length() < theItem.second)
                            return false;
                    }
                    else return false;
                }
                else return false;
            }
            return true;
        }

        std::string getTestValue(const char* aValue, bool& anError) {
            std::string temp(aValue);
            if (!anError) {
                anError = true;
                temp = std::to_string(rand() % 5) + temp;
            }
            return temp;
        }

        struct CreateTableProps {
            Errors  error;
            bool    cmdBegErr;
            bool    cmdEndErr;
            char    cmdOpenParen;
            size_t  fldPrefix;
            size_t  typePrefix;
            size_t  fldDelim;
            char    cmdCloseParen;
        };

        CommandCount addBadTable(const CreateTableProps& aProps, char aChar, std::ostream& anOutput) {
            static char prefixes[] = { ' ','0' };
            static const char fldDelims[] = { ',',' ' };

            static std::map<std::string, std::string> kFields = {
              {"id", "int NOT NULL auto_increment primary key "},
              {"name", "varchar(50) "},
              {"price", "float "},
            };

            anOutput << "create " << prefixes[aProps.cmdBegErr] << "TABLE "
                << prefixes[aProps.cmdEndErr] << "table_" << aChar << aProps.cmdOpenParen;

            size_t thePos = 1;
            for (auto& thePair : kFields) {
                anOutput << prefixes[thePos == aProps.fldPrefix] << thePair.first
                    << ' ' << prefixes[thePos == aProps.typePrefix]
                    << thePair.second;
                if (thePos < kFields.size()) anOutput << fldDelims[thePos == aProps.fldDelim];
                thePos++;
            }

            anOutput << aProps.cmdCloseParen << ";\n";

            CommandCount theCommand{ Commands::createTable,1,'>',true };
            return theCommand;
        }

        void addUsersTable(std::ostream& anOutput) {
            anOutput << "create table Users (";
            anOutput << " id int NOT NULL auto_increment primary key,";
            anOutput << " first_name varchar(50) NOT NULL,";
            anOutput << " last_name varchar(50),";
            anOutput << " age int,";
            anOutput << " zip int);\n";
        }

        void addPaymentsTable(std::ostream& anOutput) {
            anOutput << "create table Payments (";
            anOutput << " id int NOT NULL auto_increment primary key,";
            anOutput << " user_id int,";
            anOutput << " paydate int,";
            anOutput << " amount int);\n";
        }

        void addAccountsTable(std::ostream& anOutput) {
            anOutput << "create table Accounts (";
            anOutput << " id int NOT NULL auto_increment primary key,";
            anOutput << " account_type varchar(25) NOT NULL,";
            anOutput << " amount int);\n";
        }

        void addBooksTable(std::ostream& anOutput) {
            anOutput << "create table Books (";
            anOutput << " id int NOT NULL auto_increment primary key,";
            anOutput << " title varchar(25) NOT NULL,";
            anOutput << " subtitle varchar(25),";
            anOutput << " isbn varchar(10),";
            anOutput << " user_id int);\n";
        }

        //----------------------------------------------

        bool containsCmdList(const std::string& aBuffer) {
            return true;
        }

        bool aboutTest(std::stringstream& anOutput) {
            std::string theScript{ "about;" };
            std::stringstream theInput(theScript);
            return scriptTest(theInput, anOutput, [&](const std::string& aBuffer) {
                return containsCmdList(aBuffer);
                });
        } //about

        //---------------------------------------

        bool dojoTest(std::stringstream& anOutput) {
            std::cout << "I'm doing the dojo test!\n";
            return true;
        }

        //----------------------------------------------

        bool hasHelp(const std::string& aString) {
            static std::vector<const char*> theCmds{
              "create database","show databases","drop database","use database",
              "about","version","quit"
            };

            std::string temp(aString);
            std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

            for (auto thePhrase : theCmds) {
                if (temp.find(thePhrase) == std::string::npos) {
                    return false;
                }
            }
            return true;
        }

        bool helpTest(std::stringstream& anOutput) {
            std::string theScript{ "help;" };
            std::stringstream theInput(theScript);
            return scriptTest(theInput, anOutput, [&](const std::string& aBuffer) {
                return hasHelp(aBuffer);
                });
        }

        //----------------------------------------------

        bool quitTest(std::stringstream& anOutput) {
            std::string theScript{ "quit;" };
            std::stringstream theInput(theScript);
            return scriptTest(theInput, anOutput, [&](const std::string& aBuffer) {
                return matches(aBuffer, { {"quit",4},{"DB::141 is shutting down",24} });
                });
        } //quit

        //----------------------------------------------

        bool scriptTest(std::istream& anInput, std::stringstream& anOutput,
            Validate aValidator, size_t aMaxErrors = 1) {
            ECE141::AppController theApp;
            ScriptRunner        theRunner(theApp);
            theRunner.run(anInput, anOutput, aMaxErrors);
            auto theBuffer{ anOutput.str() };
            return aValidator(theBuffer);
        }

        //----------------------------------------------

        bool versionTest(std::stringstream& anOutput) {
            std::string theScript{ "version;" };
            std::stringstream theInput(theScript);
            return scriptTest(theInput, anOutput, [&](const std::string& aBuffer) {
                return matches(aBuffer, { {"version",7}, {"Version:",10} });
                });
        }

        //---------------------------------------------------

        enum states {
            wasUnknown, wasAlter, wasCreate, wasDescribe,
            wasDelete, wasDrop, wasDump, wasInsert, wasSelect,
            wasShow, wasUpdate, wasUse
        };

        //validates output of DBCommand test..
        size_t analyzeOutput(std::istream& aStream, Responses& aResults) {
            std::stack<Commands> theStack; //tracking open state...

            static KWList createDB{ Keywords::create_kw,Keywords::database_kw };
            static KWList changedDB{ Keywords::database_kw,Keywords::changed_kw };
            static KWList showDBs{ Keywords::show_kw,Keywords::databases_kw };
            static KWList dumpDB{ Keywords::dump_kw,Keywords::database_kw };
            static KWList dropDB{ Keywords::drop_kw,Keywords::database_kw };
            static KWList createTable{ Keywords::create_kw,Keywords::table_kw };
            static KWList showTables{ Keywords::show_kw,Keywords::tables_kw };
            static KWList showIndex{ Keywords::show_kw,Keywords::index_kw };
            static KWList showIndexes{ Keywords::show_kw,Keywords::indexes_kw };
            static KWList dropTable{ Keywords::drop_kw,Keywords::table_kw };
            static KWList insertInto{ Keywords::insert_kw,Keywords::into_kw };

            Tokenizer theTokenizer(aStream);
            if (theTokenizer.tokenize()) {
                TestSequencer theSeq(theTokenizer);
                int theValue{ 0 };
                while (theTokenizer.more()) {
                    //.auto &theToken=theTokenizer.current();
                    if (theSeq.clear().nextIs(createDB)) {
                        if (theSeq.skipPast(Keywords::query_kw)) {
                            theTokenizer.skipTo(TokenType::number);
                            theSeq.getNumber(theValue).skipPast(')');
                            aResults.push_back({ Commands::createDB,theValue });
                        }
                    }
                    else if (theSeq.clear().nextIs(showDBs)) {
                        if (theSeq.skip(1)) {
                            theTokenizer.skipTo(TokenType::number);
                            theSeq.getNumber(theValue).skipPast(')');
                            aResults.push_back({ Commands::showDBs,theValue });
                        }
                    }
                    else if (theSeq.clear().nextIs({ Keywords::use_kw })) {
                        if (theSeq.skipPast(';').nextIs(changedDB).skipPast(')')) {
                            aResults.push_back({ Commands::useDB, 0 });
                        }
                    }
                    else if (theSeq.clear().nextIs(dropDB)) {
                        if (theTokenizer.skipTo(TokenType::number)) {
                            theSeq.getNumber(theValue).skipPast(')');
                            aResults.push_back({ Commands::dropDB,theValue });
                        }
                    }
                    else if (theSeq.clear().nextIs(dumpDB)) {
                        if (theSeq.skipPast(Keywords::rows_kw)) {
                            if (theTokenizer.skipTo(TokenType::number)) {
                                theSeq.getNumber(theValue).skipPast(')');
                                aResults.push_back({ Commands::dumpDB,theValue });
                            }
                        }
                    }
                    else if (theSeq.clear().nextIs(createTable)) {
                        if (theSeq.skipPast(';').nextIs({ Keywords::query_kw })) {
                            theTokenizer.skipTo(TokenType::number);
                            theSeq.getNumber(theValue).skipPast(')');
                            aResults.push_back({ Commands::createTable,theValue });
                        }
                        else {
                            theTokenizer.skipTo(TokenType::number);
                            theSeq.getNumber(theValue);
                            theTokenizer.skipTo(TokenType::number);
                            theTokenizer.next(); //skip number..
                            aResults.push_back({ Commands::createTable, theValue,'>', true });
                        }
                    }
                    else if (theSeq.clear().nextIs(showTables)) {
                        theTokenizer.skipTo(TokenType::number);
                        theSeq.getNumber(theValue).skipPast(')');
                        aResults.push_back({ Commands::showTables,theValue });
                    }
                    else if (theSeq.clear().nextIs(showIndex)) {
                        if (theTokenizer.skipTo(Keywords::rows_kw)) {
                            auto theToken = theTokenizer.peek(-1);
                            theValue = std::stoi(theToken.data);
                            theSeq.skip(7);
                        }
                        aResults.push_back({ Commands::showIndex,theValue });
                    }
                    else if (theSeq.clear().nextIs(showIndexes)) {
                        theTokenizer.skipTo(TokenType::number);
                        theSeq.getNumber(theValue).skipPast(')');
                        aResults.push_back({ Commands::showIndexes,theValue });
                    }
                    else if (theSeq.clear().nextIs(dropTable)) {
                        theTokenizer.skipTo(TokenType::number);
                        theSeq.getNumber(theValue).skipPast(')');
                        aResults.push_back({ Commands::dropTable,theValue });
                    }
                    else if (theSeq.clear().nextIs(insertInto)) {
                        if (theSeq.skipPast(';').nextIs({ Keywords::query_kw }).skipPast(',')) {
                            theSeq.getNumber(theValue).skipPast(')');
                            aResults.push_back({ Commands::insert,theValue });
                        }
                    }
                    else if (theTokenizer.skipIf(Keywords::describe_kw)) {
                        if (theTokenizer.skipTo(Keywords::rows_kw)) {
                            auto theToken = theTokenizer.peek(-1);
                            theValue = std::stoi(theToken.data);
                            aResults.push_back({ Commands::describe,theValue });
                            theSeq.clear().skipPast(')');
                        }
                    }
                    else if (theSeq.clear().nextIs({ Keywords::select_kw })) {
                        if (theTokenizer.skipTo(Keywords::rows_kw)) {
                            auto theToken = theTokenizer.peek(-1);
                            theValue = std::stoi(theToken.data);
                            aResults.push_back({ Commands::select,theValue });
                            theSeq.clear().skipPast(')');
                        }
                    }
                    else if (theSeq.clear().nextIs({ Keywords::update_kw })) {
                        if (theTokenizer.skipTo(Keywords::rows_kw)) {
                            auto theToken = theTokenizer.peek(-1);
                            theValue = std::stoi(theToken.data);
                            aResults.push_back({ Commands::update,theValue });
                            theSeq.clear().skipPast(')');
                        }
                    }
                    else if (theSeq.clear().nextIs({ Keywords::delete_kw })) {
                        if (theTokenizer.skipTo(Keywords::rows_kw)) {
                            auto theToken = theTokenizer.peek(-1);
                            theValue = std::stoi(theToken.data);
                            aResults.push_back({ Commands::delet,theValue });
                            theSeq.clear().skipPast(')');
                        }
                    }
                    else theTokenizer.next(); //skip...
                }
            }
            return aResults.size();
        }

        bool hasFiles(const StringList& aFilelist) {
            for (auto theFile : aFilelist) {
                std::string thePath = Config::getDBPath(theFile);
                std::ifstream theStream(thePath);
                if (!theStream) return false;
            }
            return true;
        }

        std::string getRandomDBName(char aChar) {
            uint32_t theCount = rand() % 99999;
            return std::string("testdb_" + std::to_string(theCount + 1000) + aChar);
        }

        bool compareCounts(CountList& aList1, CountList& aList2, const size_t aPos) {
            bool theResult = true;
            for (size_t i = 0; i < aList1.size(); i++) {
                if (theResult) {
                    theResult = i == aPos ? aList1[i] < aList2[i] : aList1[i] == aList2[i];
                }
            }
            return theResult;
        }

        size_t countDBFiles() {
            std::string thePath = Config::getStoragePath();
            FolderReader theReader(thePath.c_str());
            size_t theCount{ 0 };
            theReader.each(Config::getDBExtension(),
                [&](const std::string& aName) {
                    theCount++;
            return true;
                });
            return theCount;
        }

        bool deleteFiles(StringList& aFileList) {
            for (auto& theFile : aFileList) {
                fs::remove(theFile);
            }
            return true;
        }

        //----------------------------------------------

        bool createFile(const std::string& aPath, const std::string& aName) {
            std::string temp(aPath);
            temp += '/';
            temp += aName;
            std::fstream output(temp, std::fstream::out | std::fstream::trunc);
            output << "some text\n";
            return true;
        }

        //----------------------------------------------

        bool doReaderTest() {
            std::string thePath = Config::getStoragePath();
            createFile(thePath, "test1.txt");

            FolderReader theReader(thePath.c_str());
            size_t theCount{ 0 };
            theReader.each(".txt", [&](const std::string& aName) {
                theCount++;
            return true;
                });
            return theCount > 0;
        }
        //----------------------------------------------

        bool doCreateDBTests(const StringList& aFileList, Responses& aResponses,
            std::stringstream& anOutput) {

            std::stringstream theCmds;
            theCmds << "CREATE DATABASE " + aFileList[0] << ";\n ";
            theCmds << "create database " + aFileList[1] << ";\n ";
            theCmds << "CrEaTe dAtABaSe " + aFileList[2] << ";\n ";
            theCmds << "use " + aFileList[0] << ";\n";
            theCmds << "shoW databaseS;\n";

            auto theStr = theCmds.str();
            std::stringstream theOutput1;

            theCmds >> std::noskipws; //make sure to include ws...

            bool theResult = scriptTest(theCmds, theOutput1,
                [&](const std::string& aBuffer) {
                    if (hasFiles(aFileList)) {
                        auto temp = theOutput1.str();
                        std::stringstream theInput(temp);
                        //anOutput << temp; //show user...
                        auto theCount = analyzeOutput(theInput, aResponses);
                        Expected theExpected({
                          {Commands::createDB,1}, {Commands::createDB,1},
                          {Commands::createDB,1}, {Commands::useDB,0}, {Commands::showDBs,2,'>'}
                            });
                        return theCount && theExpected == aResponses;
                    }
            return false;
                }); //script test
            return theResult;
        }

        bool doDropAndShowTests(const StringList& aFileList, int aPrevDBCount,
            std::stringstream& anOutput) {

            std::stringstream theCmds, theOut;

            theCmds << "DRop dataBASE " + aFileList[1] << ";\n ";
            theCmds << "shoW databaseS;\n";
            theCmds << "dump database " + aFileList[0] << ";\n ";
            theCmds << "drop database " + aFileList[0] << ";\n ";
            theCmds << "drop database " + aFileList[2] << ";\n ";

            theCmds >> std::noskipws; //make sure to include ws...

            bool theResult = scriptTest(theCmds, theOut, [&](const std::string& aBuf) {
                auto temp = theOut.str();
            std::stringstream theOutput(temp);
            //std::cout << temp << "\n";
            anOutput << temp << "\n"; //capture!

            Responses theResponses;
            if ((theResult = analyzeOutput(theOutput, theResponses))) {
                Expected theExpected({
                  {Commands::dropDB,0}, {Commands::showDBs, 1, '>'},
                  {Commands::dumpDB, -1,'>'}, {Commands::dropDB,0},
                  {Commands::dropDB,0}
                    });
                if (theExpected == theResponses) {
                    return true;
                }
            }
            return false;
                });

            return theResult;
        }

        bool dbCommandsTest(std::stringstream& anOutput) {
            std::vector<std::string> theFiles;
            const char theType = 'A';
            theFiles.push_back(getRandomDBName(theType));
            theFiles.push_back(getRandomDBName(theType));
            theFiles.push_back(getRandomDBName(theType));
            bool theResult{ false };
            if (theFiles.size()) {
                Responses theResponses;
                theResult = doCreateDBTests(theFiles, theResponses, anOutput);
                if (theResult) {
                    int theDBCount = theResponses[4].count;
                    theResult = doDropAndShowTests(theFiles, theDBCount, anOutput);
                }
                deleteFiles(theFiles);
            }
            return theResult;
        }

        bool filterCommandTest(std::stringstream& anOutput) {

            std::stringstream theCmds, theOut;
            std::string theDBName("db_" + std::to_string(rand() % 9999));
            theCmds << "create database " << theDBName << ";\n";
            theCmds << "use " << theDBName << ";\n";

            addBooksTable(theCmds);
            insertBooks(theCmds, 0, 14);

            addUsersTable(theCmds);
            insertUsers(theCmds, 0, 10);

            theCmds << getUserSelect("*", {});//basic
            theCmds << getUserSelect("*", { " order by zip",
                                           " where zip>92122" });
            theCmds << getUserSelect("first_name, last_name, age",
                { " order by zip desc"," limit 3" });

            theCmds << "show tables;\n";
            theCmds << "dump database " << theDBName << ";\n";
            theCmds << "drop database " << theDBName << ";\n";

            theCmds << std::endl;
            auto temp1 = theCmds.str();
            //std::cerr << temp1 << "\n---------------------------------------------------\n";

            theCmds >> std::noskipws; //make sure to include ws...

            bool theResult = scriptTest(theCmds, theOut, [&](const std::string& aBuf) {

                bool theResult{ false };
            auto temp = theOut.str();
            std::stringstream theOutput(temp);
            //std::cerr << temp << "\n"; //debugging!
            anOutput << temp << "\n"; //capture for main!

            Responses theResponses;
            if ((theResult = analyzeOutput(theOutput, theResponses))) {
                Expected theExpected({
                  {Commands::createDB,1},    {Commands::useDB,0},
                  {Commands::createTable,1}, {Commands::insert,14},
                  {Commands::createTable,1}, {Commands::insert,10},
                  {Commands::select,10},     {Commands::select,6},
                  {Commands::select,3},     {Commands::showTables,2},
                  {Commands::dumpDB,3,'>'},  {Commands::dropDB,0},
                    });
                theResult = theExpected == theResponses;
            }
            return theResult;
                });
            return theResult;
        }

        bool cacheCommandTest(std::stringstream& anOutput) {
            return false;
        }

        bool indexCommandTest(std::stringstream& anOutput) {
            return false;
        }

        bool joinCommandTest(std::stringstream& anOutput) {
            return false;
        }

        bool mutateCommandTest(std::stringstream& anOutput) {
            return false;
        }

        std::string getUserSelect(const std::string& aFields,
            const InitList& aClauses) {
            std::string theResult("SELECT " + aFields + " from Users ");
            if (aClauses.size()) {
                std::vector<std::string> theClauses(aClauses);
                auto rd = std::random_device{};
                auto rng = std::default_random_engine{ rd() };
                std::shuffle(theClauses.begin(), theClauses.end(), rng);
                for (auto theClause : theClauses) {
                    theResult += theClause;
                }
            }
            theResult += ";\n";
            return theResult;
        }

        bool selectCommandTest(std::stringstream& anOutput) {

            std::stringstream theCmds, theOut;
            std::string theDBName("db_" + std::to_string(rand() % 9999));
            theCmds << "create database " << theDBName << ";\n";
            theCmds << "use " << theDBName << ";\n";

            addBooksTable(theCmds);
            insertBooks(theCmds, 0, 14);

            addUsersTable(theCmds);
            insertUsers(theCmds, 0, 10);

            theCmds << getUserSelect("*", {});//basic
            theCmds << getUserSelect("*", { " order by zip" });

            theCmds << "show tables;\n";
            theCmds << "dump database " << theDBName << ";\n";
            theCmds << "drop database " << theDBName << ";\n";

            theCmds << std::endl;
            auto temp1 = theCmds.str();
            //std::cerr << temp1 << "\n---------------------------------------------------\n";

            theCmds >> std::noskipws; //make sure to include ws...

            bool theResult = scriptTest(theCmds, theOut, [&](const std::string& aBuf) {

                bool theResult{ false };
            auto temp = theOut.str();
            std::stringstream theOutput(temp);
            //std::cerr << temp << "\n"; //debugging!
            anOutput << temp << "\n"; //capture for main!

            Responses theResponses;
            if ((theResult = analyzeOutput(theOutput, theResponses))) {
                Expected theExpected({
                  {Commands::createDB,1},    {Commands::useDB,0},
                  {Commands::createTable,1}, {Commands::insert,14},
                  {Commands::createTable,1}, {Commands::insert,10},
                  {Commands::select,10},     {Commands::select,10},
                  {Commands::showTables,2},
                  {Commands::dumpDB,3,'>'},  {Commands::dropDB,0},
                    });
                theResult = theExpected == theResponses;
            }
            return theResult;
                });
            return theResult;
        }

        bool parseTest(std::stringstream& anOutput) {
            std::stringstream theCmds, theOut;
            std::string theDBName(getRandomDBName('C'));

            Expected theExpected({
              {Commands::createDB,1},{Commands::useDB,0},{Commands::createTable,1},
                });

            theCmds << "create database " << theDBName << ";\n";
            theCmds << "use " << theDBName << ";\n";

            addUsersTable(theCmds);

            std::vector<CreateTableProps> kProps{
                //{Errors::unknownCommand, true, false, '(', 0,0,0,')'}, //cmdbeg
                {Errors::syntaxError, false, false, '(', 0,0,0,' '}, //close paren
                {Errors::invalidTableName, false, true, '(', 0,0,0,')'}, //cmdend
                {Errors::syntaxError, false, false, ' ', 0,0,0,')'}, //openparen
                {Errors::illegalIdentifier, false, false, '(', 1,0,0,')'}, //fldprefix
                {Errors::unknownType, false, false, '(', 0,1,0,')'}, //typeprefix
                {Errors::syntaxError, false, false, '(', 0,0,1,')'}, //flddelim
            };

            char theChar{ 'a' };
            for (auto& theProps : kProps) {
                theExpected.add(addBadTable(theProps, theChar++, theCmds));
            }

            theCmds << "drop database " << theDBName << ";\n";
            theExpected.add({ Commands::dropDB,0 });

            theCmds << std::endl;
            auto temp1 = theCmds.str();
            //std::cerr << temp1 << "\n---------------------------------------------------\n"; //debugging!

            theCmds >> std::noskipws; //make sure to include ws...

            bool theResult = scriptTest(theCmds, theOut, [&](const std::string& aBuf) {
                bool theResult{ false };
            auto temp = theOut.str();
            std::stringstream theOutput(temp);
            //std::cerr << temp << "\n"; //debugging!
            anOutput << temp << "\n"; //capture for main!

            Responses theResponses;
            if ((theResult = analyzeOutput(theOutput, theResponses))) {
                theResult = theExpected == theResponses;
            }
            return theResult;
                }, kMaxErrors);
            return theResult;
        }

        bool tableCommandsTest(std::stringstream& anOutput) {
            std::stringstream theCmds, theOut;
            std::string theDBName(getRandomDBName('B'));

            theCmds << "create database " << theDBName << ";\n";
            theCmds << "use " << theDBName << ";\n";

            addUsersTable(theCmds);
            addAccountsTable(theCmds);
            addPaymentsTable(theCmds);

            theCmds << "show tables;\n";
            theCmds << "describe Accounts;\n";
            theCmds << "drop table Accounts;\n";
            theCmds << "show tables;\n";
            theCmds << "drop database " << theDBName << ";\n";

            theCmds >> std::noskipws; //make sure to include ws...

            bool theResult = scriptTest(theCmds, theOut, [&](const std::string& aBuf) {
                bool theResult{ false };
            auto temp = theOut.str();
            std::stringstream theOutput(temp);
            //std::cerr << temp << "\n"; //debugging!
            anOutput << temp << "\n"; //capture for main!

            Responses theResponses;
            if ((theResult = analyzeOutput(theOutput, theResponses))) {
                Expected theExpected({
                  {Commands::createDB,1},    {Commands::useDB,0},
                  {Commands::createTable,1}, {Commands::createTable,1},
                  {Commands::createTable,1}, {Commands::showTables,3},
                  {Commands::describe,3},    {Commands::dropTable,0},
                  {Commands::showTables,2},  {Commands::dropDB,0},
                    });
                theResult = theExpected == theResponses;
            }
            return theResult;
                });
            return theResult;
        }

        //-------------------------------------------------------

        void insertUsers(std::ostream& anOut, size_t anOffset, size_t aLimit) {
            static const char* kUsers[] = {
              " (\"Terry\",     \"Pratchett\", 70,  92124)",
              " (\"Ian\",       \"Tregellis\", 48,  92123)",
              " (\"Jody\",      \"Taylor\",    50,  92120)",
              " (\"Stephen\",   \"King\",      74,  92125)",
              " (\"Ted\",       \"Chiang\",    56,  92120)",
              " (\"Anthony\",   \"Doerr\",     52,  92122)",
              " (\"J.R.R.\",    \"Tolkien\",   130, 92126)",
              " (\"Aurthur C.\",\"Clarke\",    105, 92127)",
              " (\"Seldon\",    \"Edwards\",   81,  92128)",
              " (\"Neal\",      \"Stephenson\",62,  92121)"
            };

            anOut << "INSERT INTO Users (first_name, last_name, age, zip)";

            size_t theSize = sizeof(kUsers) / sizeof(char*);
            size_t theLimit = std::min(theSize, anOffset + aLimit);
            const char* thePrefix = " VALUES";
            for (size_t i = anOffset; i < theLimit; i++) {
                anOut << thePrefix << kUsers[i];
                thePrefix = ",";
            }
            anOut << ";\n";
        }

        void insertFakeUsers(std::ostream& anOut, int aGroupSize,
            Expected& anExpected, int aGroupCount = 1) {
            for (int theCount = 0; theCount < aGroupCount; theCount++) {
                anOut << "INSERT INTO Users (first_name, last_name, age, zip) VALUES ";
                const char* thePrefix = "";
                for (int theSize = 0; theSize < aGroupSize; theSize++) {
                    anOut << thePrefix <<
                        '(' << '"' << Fake::People::first_name()
                        << "\", \"" << Fake::People::last_name()
                        << "\", " << Fake::People::age(20, 60)
                        << ", " << Fake::Places::zipcode() << ')';
                    thePrefix = ",";
                }
                anExpected.add({ Commands::insert, aGroupSize });
                anOut << ";\n";
            }
        }

        void insertBooks(std::ostream& anOut,
            size_t anOffset, size_t aLimit) {
            static const char* kBooks[] = {
              " (\"The Green Mile\",4, \"C123-932L\")",
              " (\"The Stand\",4, \"RV36-M11B\")",
              " (\"Misery\",4, \"VI77-21K3\")",
              " (\"11/22/63\",4, \"PA45-M023\")",
              " (\"The Institute\",4, \"F94K-916M\")",
              " (\"Sorcerer\",1, \"E598-B81S\")",
              " (\"Wintersmith\",1, \"W84S-P70R\")",
              " (\"Mort\",1, \"KEJ5-27D3\")",
              " (\"Thud\",1, \"YAL4-J001\")",
              " (\"Time Police\",3, \"EK50-J001\")",
              " (\"The Mechanical\",2, \"ULRR-1320\")",
              " (\"The Liberation\",2, \"ZK95-9413\")",
              " (\"The Rising\",2, \"ECC7-6BB0\")",
              " (\"Exhalation\",5, \"18MQ-Q414\")",
            };

            anOut << "INSERT INTO Books (title, user_id, isbn)";

            size_t theSize = sizeof(kBooks) / sizeof(char*);
            size_t theLimit = std::min(theSize, anOffset + aLimit);
            const char* thePrefix = " VALUES";
            for (size_t i = anOffset; i < theLimit; i++) {
                anOut << thePrefix << kBooks[i];
                thePrefix = ",";
            }
            anOut << ";\n";
        }

        bool insertCommandTest(std::stringstream& anOutput, int aCount = 2) {
            std::stringstream theCmds, theOut;
            std::string theDBName(getRandomDBName('B'));

            theCmds << "create database " << theDBName << ";\n";
            theCmds << "use " << theDBName << ";\n";

            Expected theExpected({
              {Commands::createDB,1},    {Commands::useDB,0},
              {Commands::createTable,1}, {Commands::insert,5},
                });

            addUsersTable(theCmds);
            insertUsers(theCmds, 0, 5);
            insertFakeUsers(theCmds, 50, theExpected, aCount);

            theCmds << "show tables;\n";
            theCmds << "dump database " << theDBName << ";\n";
            theCmds << "drop database " << theDBName << ";\n";

            theCmds >> std::noskipws; //make sure to include ws...

            bool theResult = scriptTest(theCmds, theOut, [&](const std::string& aBuf) {
                bool theResult{ false };
            auto temp = theOut.str();
            std::stringstream theOutput(temp);
            anOutput << temp << "\n";
            //std::cerr << temp << "\n"; //debugging!

            Responses theResponses;
            if ((theResult = analyzeOutput(theOutput, theResponses))) {
                theExpected.add({ Commands::showTables,1 });
                theExpected.add({ Commands::dumpDB,aCount * 50,'>' });
                theExpected.add({ Commands::dropDB,0 });
                theResult = theExpected == theResponses;
            }
            return theResult;
                });
            return theResult;
        }

        bool deleteCommandTest(std::stringstream& anOutput, int aCount = 2) {

            std::string theDBName1(getRandomDBName('F'));
            std::string theDBName2(getRandomDBName('F'));

            std::stringstream theCmds, theOut;
            theCmds << "create database " << theDBName1 << ";\n";
            theCmds << "create database " << theDBName2 << ";\n";
            theCmds << "use " << theDBName1 << ";\n";

            addUsersTable(theCmds);
            insertUsers(theCmds, 0, 5);

            theCmds << "select * from Users;\n";

            theCmds << "use " << theDBName2 << ";\n";
            addUsersTable(theCmds);
            insertUsers(theCmds, 6, 4);

            theCmds << "select * from Users;\n";

            theCmds << "use " << theDBName1 << ";\n";

            theCmds << "DELETE from Users where zip=92120;\n";
            theCmds << "select * from Users;\n";
            theCmds << "DELETE from Users where zip<92124;\n";
            theCmds << "select * from Users;\n";
            theCmds << "DELETE from Users where zip>92124;\n";
            theCmds << "select * from Users;\n";

            theCmds << "drop database " << theDBName1 << ";\n";
            theCmds << "drop database " << theDBName2 << ";\n";

            theCmds >> std::noskipws; //make sure to include ws...

            bool theResult = scriptTest(theCmds, theOut, [&](const std::string& aBuf) {
                bool theResult{ false };
            auto temp = theOut.str();
            std::stringstream theOutput(temp);
            anOutput << temp << "\n";
            //std::cerr << temp << "\n"; //debugging!

            Responses theResponses;
            if ((theResult = analyzeOutput(theOutput, theResponses))) {
                Expected theExpected({
                  {Commands::createDB,1},    {Commands::createDB,1},
                  {Commands::useDB,0},       {Commands::createTable,1},
                  {Commands::insert,5},      {Commands::select,5},
                  {Commands::useDB,0},       {Commands::createTable,1},
                  {Commands::insert,4},      {Commands::select,4},

                  {Commands::useDB,0},       {Commands::delet,2},
                  {Commands::select,3},      {Commands::delet,1},
                  {Commands::select,2},      {Commands::delet,1},
                  {Commands::select,1},      {Commands::dropDB,0},
                  {Commands::dropDB,0},
                    });
                theResult = theExpected == theResponses;
            }
            return theResult;

                });

            return theResult;
        }

        //test dropping a table...
        bool dropCommandTest(std::stringstream& anOutput, int aCount = 2) {

            std::string theDBName1(getRandomDBName('F'));
            std::stringstream theCmds, theOut;
            theCmds << "create database " << theDBName1 << ";\n";
            theCmds << "use " << theDBName1 << ";\n";

            addUsersTable(theCmds);
            insertUsers(theCmds, 0, 5);
            theCmds << "select * from Users;\n";

            addBooksTable(theCmds);
            int theBookCount = 13;
            insertBooks(theCmds, 0, theBookCount);
            theCmds << "select * from Books;\n";

            theCmds << "show tables;\n";
            theCmds << "DROP table Books;\n";
            theCmds << "show tables;\n";

            theCmds << "drop database " << theDBName1 << ";\n";

            theCmds >> std::noskipws; //make sure to include ws...

            bool theResult = scriptTest(theCmds, theOut, [&](const std::string& aBuf) {
                bool theResult{ false };
            auto temp = theOut.str();
            std::stringstream theOutput(temp);
            anOutput << temp << "\n";
            //std::cerr << temp << "\n"; //debugging!

            Responses theResponses;
            if ((theResult = analyzeOutput(theOutput, theResponses))) {
                Expected theExpected({
                  {Commands::createDB,1},    {Commands::useDB,0},
                  {Commands::createTable,1}, {Commands::insert,5},
                  {Commands::select,5},      {Commands::createTable,1},
                  {Commands::insert,theBookCount}, {Commands::select,theBookCount},
                  {Commands::showTables,2},  {Commands::dropTable,theBookCount},
                  {Commands::showTables,1},  {Commands::dropDB,0},
                    });
                theResult = theExpected == theResponses;
            }
            return theResult;

                });

            return theResult;
        }

        bool updateCommandTest(std::stringstream& anOutput, int aCount = 2) {
            std::string theDBName1(getRandomDBName('E'));
            std::string theDBName2(getRandomDBName('E'));

            std::stringstream theCmds, theOut;
            theCmds << "create database " << theDBName2 << ";\n";
            theCmds << "create database " << theDBName1 << ";\n";
            theCmds << "use " << theDBName1 << ";\n";

            addUsersTable(theCmds);
            insertUsers(theCmds, 0, 5);

            theCmds << "use " << theDBName2 << ";\n";
            addUsersTable(theCmds);
            insertUsers(theCmds, 6, 4);

            theCmds << "use " << theDBName1 << ";\n";
            theCmds << "select * from Users;\n";

            std::string theZip(std::to_string(10000 + rand() % 75000));

            theCmds << "update Users set zip=" << theZip
                << " where id=5;\n";

            theCmds << "select * from Users;\n";

            theCmds << "use " << theDBName2 << ";\n";
            theCmds << "select * from Users;\n";

            theCmds << "use " << theDBName1 << ";\n";
            theCmds << "select * from Users where zip="
                << theZip << ";\n";

            theCmds << "drop database " << theDBName1 << ";\n";
            theCmds << "drop database " << theDBName2 << ";\n";

            theCmds >> std::noskipws; //make sure to include ws...

            bool theResult = scriptTest(theCmds, theOut, [&](const std::string& aBuf) {
                bool theResult{ false };
            auto temp = theOut.str();
            std::stringstream theOutput(temp);
            anOutput << temp << "\n";
            //std::cerr << temp << "\n"; //debugging!

            Responses theResponses;
            if ((theResult = analyzeOutput(theOutput, theResponses))) {
                Expected theExpected({
                  {Commands::createDB,1},     {Commands::createDB,1},
                  {Commands::useDB,0},        {Commands::createTable,1},
                  {Commands::insert,5},       {Commands::useDB,0},
                  {Commands::createTable,1},  {Commands::insert,4},
                  {Commands::useDB,0},        {Commands::select,5},
                  {Commands::update,1},       {Commands::select,5},
                  {Commands::useDB,0},        {Commands::select,4},
                  {Commands::useDB,0},        {Commands::select,1},
                  {Commands::dropDB,0},       {Commands::dropDB,0}
                    });
                theResult = theExpected == theResponses;
            }
            return theResult;
                });
            return theResult;
        }

    }; //TestAutomatic...

}


#endif /* TestAutomatic_h */



