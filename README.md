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

### Known bugs.

1.  Possible memory leak when invalid commands are entered.
2.  Shell terminates with an error if connectors are located at the end of an 
input instead of resetting.
3.  The comment symbol, "#" does not work properly when placed at the back of
a word.  For instance, "echo hello#world" prints "hello" when it is supposed
to print "hello#world".

