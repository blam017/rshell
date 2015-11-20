# Basic command shell

By Brandon Lam.

This purpose of this project is to create a simple shell which when run, can 
execute basic commands such as mkdir, ls, and echo.  This shell must also be
designed to take in connectors such as "&&" and "||" while keeping their 
respective functionalities.  This shell is supposed run exactly like a normal
shell would.

### Usage.

To use this shell, simply run the executable.  The program will then output 
a shell where you may type in commands as you would in a normal command prompt.
Typing in the word "exit" exits the program and returns you back to terminal.

#### UPDATE TO USAGE
The new shell has added functionality which includes the "test" command and 
precedence operators.

### Design
Unfortunately, this project was started with a messy and convoluted design and
there has been very little to no time to fix it for now.  The entire project is
based around functions which are called after a main loop finishes parsing 
user input.  This made extensions for the project tedious, as brand new 
functions had to be made in order to call existing ones for new operations such
as the precedence operators.  Future changes to this program could include 
switching the design pattern of the project from using disjoint functions
to using classes so that adding onto and maintaining the code will be easier.

### Known bugs.

1.  Possible memory leak when invalid commands are entered.
2.  Shell terminates with an error if connectors are located at the end of an 
input instead of resetting. (Fixed)
3.  The comment symbol, "#" does not work properly when placed at the back of
a word.  For instance, "echo hello#world" prints "hello" when it is supposed
to print "hello#world".
4.  Quotes do not work as intended or have virtually no functionality at all.
For example, echo "ls && a" should print ls && a.
5.  Possible memory leak if stat() call fails.
6.  Exit does not work if called within consecutive precedence operators.
