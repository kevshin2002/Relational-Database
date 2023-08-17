# SP23-141b-Database
## Sprint #1 -- Handling App-Level Commands

### Due April 10, 2023 at 11:30pm (PST)
Getting started with our application

## Getting Aquainted With the Code
In this assignment, you're expected to correctly process three simple commands ("about", "version", "quit"). 

Take some time this week to review the code we've provided for you. All the files are intended to serve as the basis for your own database application. 

<b>NOTE</b>: While we've provided you some starting code and suggest you take advantage of it, you ARE NOT required to do so. You're welcome to use your own code instead. You're also welcome to make any changes you like the files we've provided. Of course, any files you add to your repository must be original work (created by you or your teammate) -- you are not permitted to use code from another source.  

In order for your `AppController` to work with our testing system, it must provide the following methods:

```
virtual StatusResult  handleInput(std::istream &anInput, ViewListener aViewer);
OptString             getError(StatusResult &aResult) const;
bool                  running() const; //return false if issues 'quit' command
```


### We're using C++ 17 in this class

Make sure you have an IDE (editor, debugger) that generates code according to the C++ 17 standard. Visual Studio and XCode can both do this. You're going to be expected to work with new idioms that are only available in C++ 17.

### Github project

Once you accept the assignment you should create a Github Project for your team. Here is the video created by Stephen showing how to create a project. 


[![How to create Github Project](https://img.youtube.com/vi/ytQ9RbqCf8U/0.jpg)](https://www.youtube.com/watch?v=ytQ9RbqCf8U)

## Challenge #1 - Basic Command Processing

Your first challenge is to develop a way to 'process commands' that you recieve (in string form) from a user (via console or script). Your system will be called by the testing system via the `AppController::handleInput` method. Keep in mind that you'll recieve many types of commands, with many variations. Do not presume commands are always well formed, in a given case, or specific order. It may be wise to think about a flexible design -- rather than hard-coding a simple switch statement.

> Stop for a moment and this about what design patterns are at work here. 

You will be expected to identify the type of each command you recieve, process that command, and create a view object the represents the output of the command. If an error occurs (or you can't process the command), the view your repond with will describe the error. You'll pass the view along to a `ViewListener` so that your output can be presented.  

Begin by developing your own process for handling commands, which begins in the `AppController::handleInput(...)` method.

## Challenge #2 - The `About` Command

The first actual command you will handle is the "About" command. When issued, the `about` command will generate a response that includes the names of the members of the team that worked on your solution. Below is a sample interaction:

```
about;
Authors: Snoopy and Woodstock
```

Once again, your response to any command will be a `View` object of some type, that implements a `View.show()` method, which when called and output the results of the command to an output stream.

## Challenge #3 -  The `Version` Command

Your next challenge is to implement the "version" command. When a user types the "version" command, your system should respond with the current version of your application, which is "0.1". Your output view should report:  "Version 0.1". Each week, the version number will be incremented by 0.01, until it reaches 1.0 in week 10.

As before, your response to any command will be a `View` object of some type, that implements a `View.show()` method, which when called and output the results of the command to an output stream. Sample output is shown below:

```
version;
Version: 0.1
```

## Challenge #4 - The `Quit` Command

Your next challenge is to implement the "quit" command. When issued, the result of this command is to produce a view that indicates the system is shutting down, and to change the state of your `AppController.running` value to false. In manual-testing mode, this will cause your app to terminate the control loop and stop processing commands. Sample output is shown below:

```
quit;
DB::141 is shutting down
```

Once the user has issused the `quit` command, your `AppController` should refuse to process any further commands.

## Challenge #5 - Manual Testing Mode 

This challenge should work automatically, presuming the others have been implemented. In this challenge, you will run your application in "interactive mode" by running your program without any arguments. In the `main.cpp` file, this scenario is handled by entering, "manual testing mode" -- where the program works interactively with you as user via the console. You can manually enter commands that get sent to your `AppController` to be run.  Manual testing model will continue to operate, until the user issues the `quit` command, after which the program should terminate.

It's essential that you be able to run your system interactively this way. Please test that now.

## Engineering Notebook

From a design standpoint, describe the design patterns in use in the starting code for this application. For each pattern you identify, describe which class the pattern is found it, along with a brief description of how it's used. You should be able to identify a minimum of 10 patterns. 


Engineering Notebook will be done at a seperate assignment. Link is available both from slidespace and [here](https://classroom.github.com/a/V7dHDYdi)

## Submitting your work

Check your files using git (and check the auto-grader output) provided by Vlad-the-compiler.

### Grading

```
compile test: 10pts
about test:   30pts
version test: 30pts 
quit test:    30pts
```

> NOTE: If your compiler output has any warnings, you will receive a 0 for this assignment (even if Vlad-the-Compiler gives you 100%)

# SP23-141b-Database -- Sprint #2
#### Due April 17, 2023 at 11:30pm (PST)

## Getting Started with Database Commands

In this assignment, your team will begin to implement "Database Level" commands that relate to creating, listing, and managing database containers.  At this point, the commands don't really do much -- but we'll be setting up scaffolding to handle real functionality soon. For example, a user of your application might issue the following command, and receive a response:

```
> CREATE DATABASE Test1; 
Query Ok, 1 rows affected (0.000882 secs)
```

This will result in a new database container named `Test1` being created in a prescribed folder on your harddrive. 

> **NOTE:** Command keywords are not case senstive, we just write keywords in ALL-CAPS for emphasis.

### Getting Started

To get started, you will expand the zipfile we provided to you on canvas into your current project folder. This will overwrite some given files (like `main.cpp`, `TestAutomatic.*`, and others -- and it will add new files to your project).  

> **BE CAREFUL** -- If you made changes to files listed below (in updated files section), take a moment to make sure you have a copy, as they'll get overwritten.  

## Updated Files...

We've updated some files in this version, mostly to enhance and extend our ability to test. 

```
main.cpp           -- changed to support new tests
TestAutomatic.*    -- changed to support new tests
TestManually.*.    -- changed to support new tests
ScriptRunner.hpp.  -- changed to support more advanced tests
TestSequencer.hpp. -- changed to support more advanced tests
Tokenizer.*        -- changed to enhance functionality for testing
Errors/keywords    -- added some additional tokens
Scanner.*          -- changed to enhance functionality for testing
```

#### `AppController` Constructor Change 
The constructor for `AppController` no longer takes any arguments. Please change your current version to remove all arguments. 

#### `basictypes.hpp` change 
Please add this new type to `basictypes.hpp`:

```
  using StringList = std::vector<std::string>;
```

## New Files...

In this version of the assignment, we've added a few new files for your use.  You are NOT required to use these classes, but they will be helpful in delivering the functionaliy that is expected.  They are outlined below:

### `FolderReader.hpp` 

This class is designed to help you read the contents of a given folder. It provides a visitor method called "each", whereby a delegate can get called back with each of the files found in the given folder. Perfect for use with `FolderView` below. This class is implemented using the `std::filesystem` class provided by the C++ library. 

> NOTE: Since everying your system outputs will be in a view (except raw debugging info you send to std::cerr), you might want to make a `FolderView` class as well. It's up to you -- but in our reference implementation, this was a good way to generate the view you'll need for the `show databases` command.


### `BlockIO.hpp/.cpp` 

These (optional) files are intended to provide a low-level API for reading and writing blocks. Use this if you like.

### `Storage.hpp/.cpp` 

The `Storage` class is used for high-level streaming I/O in your application.  We will discuss this class during lecture - to help you make sense of how to use it.  Although we provided the `Storage` class declartion too you now, you really don't need it until next week.  We provided this to you early so you could start thinking about how you save "streams" of data into a database file.   

You are free to ignore this file, and develop your own approach.

### `Database.hpp/.cpp` 

These files contain a minimal starting point for your `Database` class. This class will have a significant role in future assignments. For now, it's mostly a placeholder where you can get started, but in the future it might be a good object to implement core SQL commands (like inserting, updating deleting rows, etc.). 

You're welcome to use this design, or ignore this class and develop your own.  These classes are only here to help you get started.


#### Understanding the Storage Class

> **NOTE:** We're going to cover over this concept in lecture soon. But here's an introduction to the idea. Skip for now if you prefer.

In assignment 2, we provided a class called `BasicIO` which can handle reading and writing `Block` objects, like `Row` and `Schema` objects that are guaranteed to fit into a `Block`. Storage blocks are great for `Row` and `Schema` objects because we want a very fast way to find, read, and write these objects. 

There are other cases where we have data that doesn't fit neatly into a single block. For example, imagine the case where we have an `Index` where each key/value pair occupies 8 bytes of memory. If we have a table of 1000 rows, the primary-key index for our table will require 8K bytes. Using a "block-oriented" API to read/write an `Index` would be very inconvenient. Instead, we'd like to use a standard stream interface for reading/writing our index.

**Note**: Don't worry if you don't understand what an `Index` is. It will be explained more later.

We've provided you a starting point for a new class called `Storage`. The `Storage` class is a sub-class of `BlockIO`, that provides an API to read/write data streams. This was designed to handle things like our `Index` class. Here's how you might use it to save your `Index` (of 1000 key/value pairs) to a database file.

1. Write your `Block` into a regular stream. We like using `stringstream` for this purpose.
2. Call `Storage::save(std::iostream &aStream, StorageInfo &anInfo)`, passing the buffer you created from step 1

Your `Storage:save()` method should determine how big the given stream is, and calculate how many 1K `Blocks` it would need to store that much data.  Next, it will find (or create) that many blocks. Finally, it will copy data from the given stream, one block at a time, and save it into the database file. 

You'll want to find a way to create logical "associations" between blocks that belong to a stream.  In our implementation we use extra data properties in the `Block::header` to indicate which stream the block belongs to, the order of the block in the sequence, and so forth.

Again, we'll cover this concept more in class. For now, we're just trying to get you to think about storing "streams" of data in your database.


### Command Processors

This week in Assignment#2 we're focusing on "Database level" commands (see the list below). Handling and processing of these commands is very different than handling the "Application Level" commands from your first assignment, such as `version` and `quit`. 

Since your first set of commands in PA were so basic (and easy to parse), you may have just hard-coded the way you handle them. As things get more sophisticated, this may not scale.  Also -- you'll notice that commands tend to "group together" into natural categories:

```
App Commands -- about, version, help, quit
DB Commands -- create database, show databases, use database, drop database, dump database
SQL Commands -- insert, update, delete (and more)
```

As you handle commands, you have to identify the type of command you've received, make an associated command (or statement) object, then run that to produce the desired result. This is a very well known process for command handling in an application. 

We **strongly** suggest your create a `CmdProcessor` class, name make a subclass to handle these "Database Level" commands. In our reference implementation, we called our `DBProcessor` (but you can do whatever you like) if you use this idiom.

When the user issues a "Database Level" command that your `Application` class doesn't understand, it should forward theses requests to this new "Database Level" command processor. You might choose to use "Chain-of-responsibilty" design pattern for this purpose.

<hr> 

## New Commands (The DB commands for this week)

There are six new commands to be implemented in this assignment:

```
1.  CREATE DATABASE {db-name} -- create a new DB (storage file) with the given name in your storage folder
2.  DROP DATABASE {db-name} -- delete a named DB
3.  SHOW DATABASES -- render a view of all the DB's in your storage folder
4.  USE {db-name} -- tell your app to "use" a named DB for subsequent commands
5.  DUMP DATABASE {db-name}  -- show a view of blocks in a storage file (likely empty at this point)
6.  HELP -- show a list of known commands in your system
```

The actions your application will take for each of theses command will be the same as what MySQL does. Let's discuss each of them.

#### Challenge #1:  Implement your `FolderReader` class

The `FolderReader` class is designed to read a set of files in a given folder, and call a visitor with each name.  

The tests for this assignment require that your `FolderReader` class is working. During the test, the auto-test will create a `FolderReader` and ask it to visit each file with the ".db" extension within the default storage folder.  There should be at least one (the test will generate it).  All you really have to do is implement `FoldeReader::each()`. The logic required is just a few lines of code.

#### Challenge #2: `CREATE DATABASE {db-name}`  

This command results in a new database (storage file with .db extension) being created in the prescribed storage location. All of your databases will be contained with a "storage" folder that you define. See the reference to the `Config.hpp` file above for more information.

> NOTE: Look in `config.hpp` file, and you'll see a method called `getStoragePath()`. This path indicates where on your computer the system should store database files.  We're using a macro that should automatically get a reasonable path for windows/mac. Windows users can change this to a different folder if the current setup doesn't work for you (but it should). 

We recommend (optionally) that you use the first block (blocknumber=0) to hold meta information about your database. What you choose store here (and how you encode it) is up to you. In our reference implementation, since we don't have working indexes yet, we store our "schema index" (key=name, value=blocknum) for tables in block 0.

After this commmand runs, the output you write to the `output` stream should look as shown below. You're expected to create a custom `View` class to produce this output:

```
CREATE DATABASE testing1;
Query OK, 1 row affected (0.01 secs)  
```

> NOTE: Use the provided `Timer.hpp` class to compute and output the elapsed time, that the `CREATE` command took to complete. You can get access to the global timer by using `Config::getTimer()`.  Or you can create your own timer. 

#### Challenge #3: `DROP DATABASE {db-name}`  

When the user issues the `DROP` command, they are trying to delete a known database file.  If the database being dropped is also the active database (loaded into memory), it should be removed.  Here's the sample output from this command (again, you're expected to create a custom `View` class to produce this output):

```
> DROP DATABASE {db-name}
Query OK, 0 rows affected (0.00011 secs) 
```

Your code should validate that the database file exists, and if so, remove that file from the default folder where you keep database files. Since we're using C++ 17, you can use the `filesystem::remove` function to delete the file (and this works in a cross-platform way):

#### Challenge #4: `SHOW DATABASES`  

This command is interesting, because it has no impact on the database in use (in memory). The `SHOW DATABASES` command needs to show your user a list of the databases that have been created so far (one per line). You're expected to create a custom `View` class to produce this output.  We recommend that you build a custom view called `FolderView.*` for this purpose -- but that's up to you.  Notice how this output has a very "tabular" looking output?  Well, you could get started building your `TabularView` class and use that here, if you prefer.

```
> SHOW databases;
+--------------------+
| Database           |
+--------------------+
| foo                |
| bar                |
| wiltz              |
+--------------------+
Query Ok, 3 rows affected (0.02 sec)
```

To do this, you must iterate the files in the prescribed storage location (specified in `Config.hpp ::getStoragePath()`). You need to use a library that can iterate files in a folder/directory. Fortunately, in C++ 17, we have such a library:

```
#include <filesystem>
```

The `<filesystem>` library provides platform independent function calls that let you do all kinds of different operations on files and folders on your local harddrive:

1. Iterate files in a directory/folder
2. Get path information
3. Manipulate files (copy, move, create, link...)
4. Change file permissions
5. Get size and meta data about a file
6. *and more*

Please be sure to watch this weeks video about ["Closures and Views"](https://youtu.be/yX_qRSi6huU). It can really help you with this task.

#### Challenge #5: `USE {db-name}`  

The `USE {db-name}` command makes a known database available in your application.  Generally, this means confirming the storage file exists, opening the file, and getting your application ready to process subsequent commands related to the chosen database. While you're free to implement your database any way you like, in our reference implementation - when the `use DB` statement is issued -- we create a `Database` object, initialize it to open the associated file, and we preload whatever `Schema` objects have been created in the database.  (We won't create any `Schema's` until next week). 

Here's an example of the expected output when you issue the `USE {db-name}` command:

```
> use testing;
Database changed (0.00031 secs)
```

#### Challenge #6: `DUMP DATABASE {db-name}`  

This command does not exist in MySQL -- we've created it specially just for debugging purposes.  The `DUMP DATABASE {db-name}` command should iterate through every block in the associated database storage file, and display descriptive information about each block. We have found that this command can be very helpful while you're debugging your database application.  Here is the sample output (again you're expected to create a custom `View` class to produce this output), for a database file that already has some data (yours may be empty at this point):

```
> dump database testdb;  
+----------------+--------+---------------+
| Type           | Id     | Extra         |
+----------------+--------+---------------+
| Meta           | 0      |               |
-----------------+--------+---------------+
| Schema         | 1      | Users         |
-----------------+--------+---------------+
| Data           | 1      | Users         |
-----------------+--------+---------------+
| Free           | 0      |               |
-----------------+--------+---------------+
| Data           | 3      | Users         |
-----------------+--------+---------------+
Query Ok, 5 rows affected (0.02 sec)
```

**NOTE:** You are free to output any information you like for each block. However, the first column MUST refer to the block type, and describe the block according to the types you have defined. By default, we've defined block types of "Meta", "Schema", "Data", "Index", and "Free". 

A `BlockType enum` has been declared for you in the `BlockIO.hpp` file. You may change this enum list to anything you like, or replace it altogether.  

For example, in our example we appended two columns (Id and Extra). Our database implementation uses this information to track/link related data elements. You are not required to use them, and can ignore the following explanation if you don't intend to do so.

When our block is a `Schema` type, the `Id` column represents the hash value of that `Schema` (if you choose to use this technique). If the block is `Data` type, `Id` will refer to the primary key value of the associated `Row`. This helps us quickly idetify the row without parsing the payload from the block. Again, you don't have to use this methodology if you don't want to do so.

Again, in your output, you MUST include the `Type` column but are free to append additional columns that help your debugging.

#### Challenge #7: `HELP`  

This very simple application level command simply prints out all the commands your application currently supports. For example:

```
> help;
create database {name}: make a new database in storage folder
drop database {name}: delete a known database from storage folder
show databases: list databases in storage folder
use database {name}: load a known database for use
about: show members
help: show list of commands
quit: stop app
version: show app version
```

<hr> 

## A Note About Console Output

### Logging 

The testing environment is setup to capture the output of your application as it runs.  

### Debugging 

For debugging purposes, when you want to send output directly to the terminal (not logged), you can write to the `std::cerr` or `std::cout`. 

> **NOTE:** Remember that the "version number" of your `Application` should be incremented each week. This week, your `Application` should report that it is running "Version 0.2". 
> 
<hr> 

## Submitting Your Work by Monday, April 17, 11:30pm (PST)

As always, you will submit your work by pushing changes to your team repository.  You may do so as many times as you wish before the assignment is due. Each time you submit, our auto-grader will test your code according to the 5 challenges. 

> **NOTE:** DON'T FORGET to include your "AboutUs" class -- we use this class to determine who will get credit for the assignment. 
 
### Grading 

```
1. Compiles:  5% 
2. App Test:  10% (from last week)
3. DBCommands Test: (Create, use, show, drop, dump): 85%
```

Good luck!

# SP23-141b-Database - Sprint #3
#### Due Monday, April 24, 11:30pm (PST)

## It's time to start building tables!

In this assignment, we're going to start handling "table-related" commands.  In order to so so, we're going to introduce our 3rd and final CommandProcessor: `SQLProcessor`.  This class will provide support for four new commands:

1. create table `table-name`
2. drop table `table-name`
3. describe `table-name` 
4. show tables

## Integrating given files from assignment #3 

We've added a few new files for this assignment. Just drop them into your existing project.  The following files already exist your project, but these are updates so replace your current version with these:

1. main.cpp
2. TestAutomatic.hpp
3. ScriptRunner
4. TestSequencer

These files all have slight changes in Assignment #3, and should not be overridden.

## New Classes in This Assignment 

### The `Schema` class 

Consider the situation where a user wants to create a database table, by issuing a "create table..." command shown below: 

```
CREATE TABLE tasks (
  id INT AUTO_INCREMENT PRIMARY KEY,
  title VARCHAR(100) NOT NULL,
  price FLOAT DEFAULT 0.0,
  due_date TIMESTAMP, //2020-04-15 10:11:12
  status BOOLEAN DEFAULT FALSE,
)
```

The `Schema` class is an in-memory representation of a table defintion. It's a container class, that contains one-or-more `Attributes`. 
In this example, we defined a table with 5 attributes (task_id, title, price, due_date, status).  We provide you a (nearly) empty version of this file -- and you are free to make any changes you want. 

### The `Attribute` class

An `Attribute` represents a single "field" in a database table defintion (`Schema`). In the table defintion shown above, `task_id` is an example `Attribute`.  The attribute class needs to be able to store the following information:

```
- field_name
- field_type  (bool, float, integer, timestamp, varchar)  //varchar has length
- field_length (only applies to text fields)
- auto_increment (determines if this (integer) field is autoincremented by DB
- primary_key  (bool indicates that field represents primary key)
- nullable (bool indicates the field can be null)
```

We provide you an empty version of this class file, and you are free to make any changes you want. 

### The `Row` class

You are given a new `Row` class. This will hold user data for a given row. The `Row` class will also participate in the storage process. Eventually each `Row` will be stored in a `Block` inside the storage system, and retrieved later to be shown in a `TableView`.  You won't need this until next week.

> Note: At the bottom of `Row.hpp`. we've pre-defined a class called `RowCollection` that you'll use in subsequent assignments. 

> Hint: You may have guessed that `Row`, `Attribute`, and `Schema` are related in many ways. There must be a way of establishing and efficient and meaningful relationship between them.

### The `TabularView` class

A `TabularView` class is used to present user data (rows) as a result of a user issuing a `select...` statement.  You can (but are not required) to use this class to implement the `show tables` command. This class was added here as a placeholder, and to get you started thinking about how you might implement tabular views.

As you're building this class, consider how the `Schema` class might be helpful. Also for previous Views, consider how functionality can be reused from past code or past code can be refactored to reuse this View.

## Implement the Following System Commands 

### Challenge 1: "create table `table-name`" 

When a user issues this command, your "CmdProcessor" will make a corresponding statement object (subclass), that will try to parse the given input. If the input is valid, your code should construct an `Schema` object from that input. Then you'll ask the `Database` to save the given `Schema` in a block within the storage system.  This will result in the `Schema` object being encoded into a `StorageBlock` and saved in a DB file. 

```
> create table test1 (id int NOT NULL auto_increment primary key, first_name varchar(50) NOT NULL, last_name varchar(50));
Query OK, 1 row affected (0.002 sec)
```

One thing to consider -- you will frequently need to retrieve the `Schema` for a given table from storage in order to perform other actions (like inserting records). How will you quickly find and retrieve the `Schema` for a given table from storage when you need it later?  You could, of course, perform a linear scan of every `Block` in the db storage file and try to locate the `Schema` for a named table. But that would be terribly slow in a large database.  Work with your partner (or your TA) to consider your options on ways to solve this. In class, we discussed the possibility of using an `Index` class that maps key/value pairs for each schema (key=schema_name, value=block_number);

> Note: When you save an `Schema` in the storage file, the associated "block-type" will be a "SchemaBlock". If you were to use the `dump database {name}` command from last week, an "SchemaBlock", make sure your output can distinguish an "SchemaBlock" from a "Meta" or "Data" block by properly setting the `Block.header.type` field.

#### Parsing `Create Table`...

Parsing the `create table foo ()` command will be more challenging that other commands you've processsed so far. Fortunately, it follows a simple pattern shown as tokens. So...we see "create", "table", "{tablename}", "(" -- followed by list of attributes, then closed with ");". We definitely want to build little helper classes to deal with these things. Possibly the `Statement` classes mentioned in lecture?

```
ATTRIBUTE is...
  [name] [type] [optional args] 
  
CREATE TABLE is...
[create] [table] [(] 
  [attribute] [optional comma]
    ...more attributes separated by commas...
[)] [;]
```

#### Error Handling 

If by chance a given command is malformed or incomplete, you must return a well-formed error message. Here's an example:

```
create table 0123foo (id int auto_increment primary key, name varchar(50));    //NOTE BAD TABLE NAME!
Error 101: Identifier expected at line 1
```

Your error message must include the error code, message string, and line number (of the input script).  It's ok if you just use 1 for the line number for now.  

NOTE: Part of the grading for this assignment will be to test your error handling during parsing.


### Challenge 2: "show tables" 

As we discussed in class -- when a user issues this command in a SQL database, the list of known tables (from the database in use) are shown to the user in the terminal.  You will implement that now.   Let's assume that your user created a few tables. Your output should be similar to what you see in MySQL:

```
> show tables;
+----------------------+
| Tables_in_mydb       |
+----------------------+
| groups               |
| users                |
+----------------------+
2 rows in set (0.000025 sec.)
```

> Note: This output looks similar to something else we've outputted. Possibly that could be reused.

1. Create a `ShowTables` class that can recognize and parse this type of command
2. Have one of your command processors handle this command, create the `ShowTables` statement object, and handle routing as usual 
3. If all goes well, ask a controller object (maybe the `Database`?) to exectute the command and get the list of known `Schemas` 
4. Consider building a `TableView` class, and let that present a view that contains the list of tables in the DB 

### Challenge 3: "drop table `table-name`"

When your user issues this command, then your system should attempt to locate and delete the associated `Schema` (table) from the storage system. If the table doesn't exist, report an "unknown table" error to your user.  

```
> drop table groups;
Query OK, 0 rows affected (0.02 sec)
```

> **NOTE:** The number of rows affected depends on how many data rows are stored in the database for that table. 

As we discussed in lecture, "deleting" a `Block` means to set (and save) the `Block.header.type`='F' (for free). 

> NOTE: If a user deletes a table that has associated records (or indexes), these elements must also be deleted from Storage. This isn't a concern today, because we can't add records yet. But give this some consideration for a future assignment.

1. Create a `DropTableStatement` class (subclass of `Statement`) that can parse this syntax
2. Let a given CommandProcessor process and route this command
3. Assuming this given `table-name` actually exists, ask the `Database` object to delete that `Schema` and associated data.
4. Report the results back to your user (see example above)

### Challenge 4: "describe `table-name`"

When your user issues this command, then your system should attempt to locate and describe the associated `Schema`. If the table doesn't exist, report an "unknown table" error to your user.

As we discussed in lecture, "describing" a `Schema` means to print out a textual description of the attributes of the given table. For example, if we were to issue the command "describe tasks" (using the table description above) we expect to see. Note, the columns are expectd, but presentational decisions (like the widths of the columns) are up to you.

```
> DESCRIBE tasks;
+-----------+--------------+------+-----+---------+-----------------------------+
| Field     | Type         | Null | Key | Default | Extra                       |
+-----------+--------------+------+-----+---------+-----------------------------+
| id        | integer      | NO   | YES | NULL    | auto_increment primary key  |
| title     | varchar(100) | NO   |     | NULL    |                             |
| price     | float        | YES  |     | 0.0     |                             |
| due_date  | date         | YES  |     | NULL    |                             |
| status    | boolean      | YES  |     | FALSE   |                             |
+-----------+--------------+------+-----+---------+-----------------------------+
5 rows in set (0.000043 sec.)
```

1. Create a Command/Statement class that that can parse this statement
2. Let one of your command-processors route this command
3. Assuming this given `table-name` actually exists, ask the `Database` object to handle this command 
4. Consider creating a `SchemaView` to show the table description (shown above)


## Testing

As before, we recommend you perform testing on your solution by creating a test script. You can also use the auto-grader provided on github classroom for this assignment.

> **NOTE:**: Update the output value for your version command, so to that this application is version 1.3. 

## Grading

```
Parse-test: 20pts  //full of errors you need to report...
Table-test: 80pts
```

## Submitting Your Work by Monday, April 24, 11:30pm (PST)
You and your partner should make sure you turn your solution in on time!  

Good luck everyone!

# SP23-141b-Database - Sprint #4
#### Due May 1, 2023 @11:30pm (PST)

## Overview -- Let's Insert Records!

In this assignment, we are going to implement the `Insert into` command that will allow us to add records to our database. As usual, this will build on the work we've been doing in prior assignments in the `SQLProcessor`, `Database` and `Schema` classes.

Let's get started!

> **NOTE:** Be careful not to overwrite the files we provided with prior versions.

<hr>

## Key Classes in This Assignment 

You'll discover that the following classes are significant in this assignment:

### The `Value` class

We need a class that can represent each "field" of user data in a record/row. In lecture we've been discussing the use of the `std::variant` class for this purpose.  Our value class can simply be the `std::variant`, or you can create your own custom class, and add some extra utility (helper) functions. 

### The `Row` class

A `Row` is a collection of user data. Imagine that the user creates a `Users` table and inserts a few records. If they subsequently issue a `SELECT * from Users where  id=1` statement -- the database will return a collection of `Row` objects for this purpose. The row will contain a collection of key/value pairs (fieldnames + values). 

### The `Schema` class

When a user inserts a record into a table, numerous validations must occur. We must ensure that the specified table exists, all the fields specified in the `insert` statement are declared in the `Schema` and the data in the insert statement match the attributes of the `Schema`.  The `Schema` class is essential for both of these tasks.

We recommend that you take advantage of the `BlockIO::writeBlock()` method for storing your new data rows. Data rows are meant to be stored as a single block. 

<hr>

## Challenges for Assignment #4

### Challenge #1: Insert Into... 

The `insert` command allows a user to insert (one or more) records into a given table. The command accepts a list of field, and a collection of value lists -- one for each record you want to insert. Below, is an example where we insert two records -- for our very own TA's!  `<applause>` 
    

```
INSERT INTO users 
  ('first_name', 'last_name', 'email') 
VALUES 
  ('Stephen','Kim', 'sskim@ucsd.edu'),
  ('Emin', 'Kirimlioglu', 'ekirimli@ucsd.edu');
```

For this assignment, your code must handle the following tasks:

1. given an "insert..." command - one of your processors should create an `InsertStatement`  statement 
2. The `InsertStatement` should parse and validate the input (example given above); validation includes ensuring that the table specified in the command is a known table in the active `Database`
3. Assuming a valid statement and a known `Schema`, your processor(s) should route this command a `controller` object (e.g. `Database`) to be executed
4. A new `Row` should be constructed - and assigned a new primary key (type int). 
5. Think carefully about how your system will manage primary keys for given table. Each time you insert a new `Row` it will need an integer value for its primary key field.  Some object in your system needs to be responsible for generating primary key values.  Keys are never reused.
6. The new `Row` should be encoded/saved into to storage, and "associated" with the given table. This means that a new data block is added to the storage file, and that it contains attributes that associated the data row with the given table. Considering storing this information in the header portion of the `Block` used to store the data. 

This seems like a lot, right?  We'll discuss how to break this down into something you can handle in lecture this week. 

#### Encoding Data
At some point this is process, you'll create a `Row`, with a set of key/values (attribute-names=>user data). Each row will need to be saved into `Block` in your storage file. In order to do that, the `Row` object (in memory) needs to be encoded into a format, suitable to be stored in the payload of a `Block` element.

We've had lots of practice encoding data in this course. C++ strongly favors using << and >> operators for this purpose, but you are free to use another approach if you like.  Don't overthink this step. It can be very easy if use the techniques we've practiced.  If you find yourself over-engineering this in your design, stop and talk to your partner, or your TA, or ask a question in class!

### Challenge #2: Preparing for `select * from table` (for next week) [OPTIONAL]

Next week (assignment #5), your team will implement a basic vesrion of the `select` command.  Ordinarily, a database would attempt to load rows associated with a table by iterating an index. Since we don't yet have a general purpose indexing mechanism for our database, we need an alternative.

As we discussed in lecture, your `Storage` class offers a `Storage::each(visitor)` method. Many of you used that method to implement the `dump database foo` command last week. As we've shown in the class a "visitor" pattern as implemented in `Storage::each` can be combined with either functors or closures to make easy work of iterating/searching all the blocks in a database storage file. 

Let's presume we've created a "Users" table and inserted some records (Challenge #1). Consider what happens when someone issues the command, `SELECT * from Users`. You'll need to iterate the blocks in your DB storage file, and find those that are associated with the given table (users). 

#### Question 1
How will you find blocks that are associated with the given table?  Can the existing `Storage::each()` function be helpful here?

#### Question 2 
How do you convert a `Block` from a DB storage file back into a `Row` object (and vice versa)?

#### Question 3
How will you build a collection of associated `Row` objects specified in the given `select * from Users` query?

Considering these questions can help you correclty implement the `Insert INTO...` statement, and prepare to implement a basic `SELECT` in assignment #5.

<hr>

## Testing This Assignment

> **NOTE**: Make sure you update the "version" value for your `Version` command. The new version number should be 0.4. 

For our autograder, we're going to validate your `insert` command is working by monitoring the state of your storage file (.db).  A sample test might look like this:

```
create database foo;
use database foo;
create table users ... (etc.);
insert into users (field1, field2...) values (record-1-values), (record-2-values);
dump database foo;
```

The output of your dump statement should show something like this (we don't validate the columns you choose to output):

```
> dump database foo;
+------+-----------+-----+
| #    | Type      | Id  |
+------+-----------+-----+
| 0    | meta      |     |
| 1    | schema    |     |
| 2    | data      | 1   |
| 3    | data      | 2   |
+------+-----------+-----+
Query OK, 4 rows in set (0.00024 secs)
```

As always you can use the auto-grader to help get your code to perform as expected.

## Grading
- You have implemented the Insert Into statement
- You can add rows to the storage 
- Your code passes the auto-grader tests.

```
Compile-test : 0pts
App-test: 0pts
DBCommands: 0pts
Table-test: 0pts
Insert-test: 75pts
Overfill-test: 25pts //where we insert > 1000 rows
```

## Submitting Your Work by Monday May 1, 2023 - 11:30pm (PST)
You and your partner should make sure you turn your solution in on time! 

Good luck everyone!

# SP23-141b-Database - Sprint #5
#### Due Sunday May 14, 2023 - 11:30pm (PST)


## Overview -- Let's Select Records!

In this assignment, we are going to implement code to handle "select" commands like, `SELECT * from Users`. When run, this commmand will allow us to retrieve records from our database associated with a given table. as well as ORDER our data, and limit the number of rows we return. This will build on the prior work we've been doing in previous assignments in the `SQLProcessor`, and `Schema` classes. 

> This assignment is slightly bigger, so we'll give you a few extra days to complete the challenges.

Let's get started!


<hr>

## Key Classes in This Assignment 

You'll discover that the following classes are significant in this assignment:

### The `Row` class

As with the case of inserting records, we'll need a working version of our `Row` class to be able select records.

### The `Rows` (or `RowCollection`) class

The `Rows` class will contain 0 or more `Row` objects. This is what actully gets returned by your `Database` object when it performs a `select` command.  

### The `SelectStatement` class

The `SelectStatement` class will perform the common task of helping to parse `SELECT...` commands. You'll implement this class in a manner similar to the other statement classes you have built for previous assignments.   

### The `Storage` class

Since we don't yet have an index, we'll use the `Storage::each()` (visitor) function to iterate blocks in a DB file, and find `Block`s that are associated with a given table/schema. You've probably already used this method for the `DUMP` and `SHOW TABLES` commands. 

### The `DBQuery` class

The _optional_ `DBQuery` class is used to describe (and help exectute) `SELECT`, `UPDATE` and `DELETE` statements.  Note that this class is provided as a starting point. You'll need to develop it further if you want to use them.

### The `Filters` class

The _optional_ `Filters` class can be used to help you deal with queries that include expressions in a "WHERE" clause. This works in conjunction with other classes we've provided started code for -- including `Operand` and `Expression`.  Note that these classes are provided as a starting point. You'll need to develop them further if you want to use them.

### The `ParseHelpers` class

We've provided an updated version of the `ParserHelpers.*` class. This includes tools to aid with dealing with  `SELECT` statements.


### The `TabularView` class

A `TabularView` will be the class that shows a view  of a `RowCollection`  when a user issues `SELECT * ...` command. 

<hr>

## Challenge #1 : Implement the `SelectStatement` Command

The `SELECT` command allows a user to retrieve (one or more) records from a given table. The command accepts one or more fields to be retrieved (or the wildcard `*`), along with a series of **optional** arguments (e.g. `ORDER BY`, `LIMIT`).  For example:
    
```
SELECT * from Accounts order by last_name;
```

When a user issues a `SELECT...` command, you'll create a command class to handle it (e.g. `SelectStatement`), and have it parse the underlying tokens to validate the command. Many of your previous command classes stored state information, to be used when the statement was eventually run in a controller.  Since there are so many options for the `SELECT` command, we recommend storing the options in a call we call `DBQuery`. We'll talk about the many advantages of doing this in lecture. You are free to use this idiom or not.

> We provided you a starter file for the `DBQuery` class.

## Challenge #2 : Write a Controller Function to Load Selected Rows/Fields 

Presuming you can successfully parse  `SELECT` commands, the next step is to evaluate that command, and assemble a collection of `Rows` from your database that match the given query.  This command handler will be attached to one of your controllers like your other commands. We recommend the `Database` object itself,  but the choice is yours.

The key to this challenge is to correctly evaluate the properties you stored when you parsed the `SELECT...` command. There are several facets to consider, as discussed below.

### Loading Rows from Storage

You must be able to access and load all the `Rows` associated with a selected table into memory.  Since we don't yet have indexes, the task of loading rows associated with a table will require that we iterate all storage blocks. Fortunately, our `Storage` class provides the `Storage::each()` (visitor) method for cases of this nature. At this stage, you have to load _all_ the rows associated with a table. Later in this process, you will apply optional filters given to eliminate rows that don't meet given criteria.

For every data `block` you find in storage, verify that the block is associated with the table/schema specified in the `Query`. Decode each of these `Blocks` into a `Row` object, and store each of them in the `Rows` argument (type: `RowCollection`).

## Challenge #3 : Implement the `TabularView` class

In this challenge, you'll construct a `TabularView` to "show" the selected `Rows` to the user. Your `TabularView` should use the `RowCollection` you assembled in the `Database::selectRows()` method as the list of `Row` objects to show to the user. 

### Field Selection

You must be able to select _some_ or _all_ of the fields in a given table, depending on whether the command specified all fields (`SELECT *...`), or individual fields in a command separated field list (`SELECT first_name, last_name from Users...`). How you do this is up to you. You could remove unwanted fields from the `Rows` you've loaded, or deal with this step logically when you create your final `TabularView`.

> NOTE: In this assignment, you don't need to worry about table joins, but you will eventually. It might be helpful to consider the impact of that scenario as you're designing your solution.  For example, if you are JOINing data, you'll likely have to load `RowCollections` for multiple tables, and combine them into a "projected" (or aggregate) table.


## Challenge #4 : Add Database/Controller Logic to Run the "Select" Command

So...you can parse the `SELECT...` statement and create an associated `Command` object?  Great!

Now it's time to implement logic in your controller (e.g. `Database`) to actually perform the action associated with this command.  In our reference implementation, we called this method, `runSelect` -- but you can call it whatever you like.  We stronly encourage you to design your code to use the `DBQuery` class we've started for you (and discussed above and in lecture).  This makes dealing with the set of optional arguments for this command much easier.  

When your controller runs the `runSelect` method, our goal is to generate a list of `Rows` as specified in the query. Since we don't have a real `Index` class yet, you may have to interate your database storage file, in search of blocks that contain data rows that are related to this query.  

As a final step, you'll present the `TabularView` of data `Rows` to the user. Below is a sample of the expected output:

```
SELECT * from Users;
+-------+----------------+----------------+-------+--------+
| id    | first_name     | last_name      | age   | zipcode| 
+-------+----------------+----------------+-------+--------+
| 1     | Terry          | Pratchett      | 81    | 20125  |
+-------+----------------+----------------+-------+--------+
| 2     | Ian            | Tregellis      | 58    | 83311  |
+-------+----------------+----------------+-------+--------+
| 3     | Jodi           | Taylor         | 52    | 48176  |
+-------+----------------+----------------+-------+--------+
3 rows in set (0.000409 secs.)
```

## Challenge #5 : Select...WHERE ... 

This version of the `SELECT` command builds upon the work we've already done. In this case, however, we expect the the records to be filtered according to the given WHERE clause, using a `Filter` object.

```
SELECT * from Users where first_name="Adele";
```

For this task, your code must handle the following tasks (some are repeated from earlier steps):

1. Retrieve `Rows` as a `RowCollection` as you did for the basic `SELECT` case (above)
2. Filter the collection of rows to exlude those that don't pass the conditional logic (using a `Filter`)
3. Present the user data using your `TableView` class in the specified order

> NOTE: You may be asked to handle these optional clauses in any combination or order. For example, the following queries produce identical results:
```
SELECT first_name, last_name from Users LIMIT 3 order by last_name 
SELECT first_name, last_name from Users order by last_name LIMIT 3
```

### Implementing The `Filter` Class

The `Filter` class provides support for conditional logic. We'll use it to perform the conditional checks in the `SELECT..WHERE` clause. The filter class can contain one or more `Expression` objects. An `Expression` contains two `Operands` and one logical `Operator` (e.g. first_name="Megan"). The purpose of this class is to determine whether a row meets the criteria given by the user. Consider the query:

```
SELECT * from Users where age>20 AND zipcode=92100 ORDER BY last_name;
```

The `Filter` class might contain multiple expressions (age>50) (zipcode=92100) and one logical operator (AND). Records from the specified table are loaded (from earlier step), and tested by the given Filter. If a row matches the expression criteria specified in the `Filter`, it will be added to the `RowCollection`.  

We've provided a starter version of the `Filter.hpp` file. You're free to use (or discard) this class. However you will need to implement a `Filter` class so that your code can properly filter records based on values set in the `SELECT` (or other) statements.

> **NOTE**: The `Filter` class can handle basic expressions, but it doesn't (yet) know how to deal logically combining them (AND, OR, NOT).  You and your teammate need to add logic to the `Filter` class to support logically combining expressions. 


### Implementing the Comparison Logic

Also included in this assignment is a file called `Compare.hpp`.  This file provides template functions that implements the "Equals" comparision operator. While we have provided you starting code for the `Equal` condition, you must implement your own variations for the other logical operations (LessThan, GreaterThan, NotEqual, LessThanEqual, GreaterThanEqual,...). 


## Challenge #6 : Select...ORDER BY `fieldname`... 

In this challenge, you'll handle ordering the resulting collection of Rows according to the "ORDER BY" field provided. Your database does not store data in an ordered format -- so we must do this step manually before we present the results to the user. 

```
SELECT last_name, email from Users ORDER BY age;
```

You'll only be asked to order data by a single field, and only in ascending order (smallest first).

## Challenge #7 : Select...LIMIT _N_... 

In this version of the `SELECT` command we are also expected to limit the number of records retrieved. Assuming a table has 100 records, the `LIMIT` clause applies a limit of the total records retrieved, so we might only ask to retrieve N records.

```
SELECT last_name, email from Users LIMIT 5;
```

For this task, your code must handle the following tasks:

1. Retrieve `Rows` as a `RowCollection` as you did for the basic `SELECT` case (above)
2. Stop adding rows once you have reached the given LIMIT value
3. Present the user data using your `TableView` 

<hr>

## Testing This Assignment

For our autograder, we're going to validate your `select` commands are working by retrieving records from a previously created table. Let's assume we already created a Users table and added 2 records. Next we'll issue the `SELECT` command:

```
SELECT * from Users;
```

The output of your select statement should show something like this:

```
> SELECT * from Users;
+--------------------+--------------+
| id  | first_name   | last_name    |
+-----+--------------+--------------+
| 1   | emin         | kirimioglu   |
| 3   | rick         | gessner      |
| 5   | stephen      | kim          |
+-----+--------------+--------------+
3 rows in set (0.00231 sec)
```

We will also test `SELECT` commands with different filters. For instance, we'll issue a command like:

```
> SELECT id, first_name, last_name, zipcode from Users where zipcode>92120 order by zipcode LIMIT 2
+--------------------+--------------+--------------+
| id  | first_name   | last_name    | Zipcode      |
+-----+--------------+--------------+--------------+
| 3   | Anirudh      | Swaninthan   | 92126        |
| 1   | Pu           | Cheng        | 92127        |
+-----+--------------+--------------+--------------+
2 rows in set (0.00123 sec)
```

As always you can use the auto-grader to help get your code to perform as expected.

All of the tests for this assignment will be based on properly gathering the correct records (and number of records) for the given query. The queries won't be any more difficult than the ones shown above. Make sure you compiler output is free from warnings.

#### Grading
```
- Select Test           40 pts
- Filter Test           60 pts
```

## Submitting Your Work by Sunday May 14, 2023 - 11:30pm (PST)
# SP23-141b-Database - Sprint #7
#### Due Monday May 22, 2023 - 11:30pm (PST)

## Update and Delete 

In this assignment, we are going to implement the `update` and `delete` commands that will allow us to change records in our database. This will build on the prior work we've been doing in previous assignments in the `SQLProcessor`, `Entity`, and `Database` classes. You may find it helpful to reuse as much of the previous log as possible. Depending on implementation, you may be able to make use of the `select` command.

<hr>

## Key Classes in This Assignment 

You'll discover that the following classes are significant in this assignment:


### The `UpdateStatement` class

```
UPDATE Users set zipcode="12345" WHERE id=10;
```

Your team will create an `UpdateStatement` command, similar to the other commands you've created in your database. This statement will handle the `UPDATE` command. If you planned your code carefully, you can reuse parsing code you developed for the `SelectStatement` class to deal with the `WHERE` clause. 


### The `DeleteStatement` class

```
DELETE FROM Users WHERE zipcode>92000;
```

Your team will implement the `DeleteStatement` command. Once again, you might consider how this command can take advantage of code you wrote for the `SelectStatement`. 


<hr>

## Implementing Commands for Assignment #7

### Challenge #1:  The `UPDATE table` Command

The `UPDATE` command allows a user to select records from a given table, alter those records in memory, and save the records back out to the storage file.     

```
UPDATE Users SET "zipcode" = 92127 WHERE zipcode>92100;
```
 
For this challenge, your code must handle the following tasks:

1. The `UpdateStatement` should parse and validate the input (example given above); validation includes ensuring that the table specified in the command is a known table in the active database. It must also gather conditional filters.
2. Assuming a valid statement and a known `Schema`, your processor(s) should route this command to a `controller` object (e.g. `Database`) to be executed
3. A controller class (maybe `Database`) will implement the `updateRows` method, where it will work with the `Storage` class to retrieve records in the given table that match the (optional) set of given `Filters`. 
4. Iterate the selected records and change them according to given key/value pairs specified by user
5. Store the updated `Rows` back into the database into their original block
6. Provide output to the user indicating success or error of the command

```
UPDATE Users SET "zipcode" = 92127 WHERE zipcode>92100;
Query Ok. 2 rows affected (0.000087 sec)
```

> NOTE: If the `UPDATE` statement doesn't include filters (`WHERE...`), the given changes will apply to every row in the table.


### Challenge #2:  The `DELETE FROM table WHERE..` command

```
DELETE FROM Users WHERE zipcode>92000;
```

The `DELETE` command allows a user to select records from a given table, and remove those rows from `Storage`.   When a user issues the `DELETE FROM...`  command, your system will find rows that match the given constraints (in the `WHERE` clause). Next, you will remove the associated rows from storage by marking the related data blocks as `FREE`.    

For this challenge, your code must handle the following tasks:

1. The `DeleteStatement` should parse and validate the input (example given above); validation includes insuring that the table specified in the command is a known table in the active database. It must also gather conditional filters.
2. Assuming a valid statement and a known `Entity`, your processor(s) should route this command to a `controller` object (e.g. `Database`) to be executed
3. A controller class (maybe `Database`) will implement the `deleteRows` method to achieve the `DELETE` operation.
5. Use the `Storage` class to mark the blocks associated with each selected row as `FREE`. 
6. Provide output to the user indicating success or error of the command

```
DELETE from Users where zipcode>92124;
Query Ok. 1 rows affected (0.000023 sec)
```

> NOTE: If the user omits the `WHERE` clause, then you should delete _all the rows_ associated with the given table. 

### Challenge #3:  Updating the `DROP` table command

Now  that we can delete rows from our database, we need to update our `DROP TABLE ...` command do delete any rows associated with the table we are dropping. Fortunately, we can reuse our `DELETE FROM table` logic -- which will delete every row in the database associated with this table.  Assuming we have a "Users" table with 5 rows, your output will look list this (your time may vary):

```
drop table Users;
Query Ok. 5 rows affected (0.000023 sec)
```
> Note:  If you carefully plan out the logic for your `DELETE FROM table...` command, you can reuse this logic to automatically delete all the rows associated with a table that you are dropping. 

<hr>

## Testing This Assignment

As always you can use the auto-grader to help get your code to perform as expected.  All of the tests for this assignment will be based on properly gathering the correct records (and number of records) for the given query. The queries won't be any more difficult than the ones shown above.

#### Grading
```
- Insert Test 10pts
- Select Test 10pts
- Update Test 35pts
- Delete Test 30pts
- Drop Table Test 15pts
```

## Turning in your work  by Monday May 22, 2023 - 11:30pm (PST)

Make sure your code compiles, and meets the requirements given above.

