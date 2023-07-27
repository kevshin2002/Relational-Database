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



