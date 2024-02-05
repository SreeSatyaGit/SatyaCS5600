# Project Title

Programming Assignment -1

## Overview

This program implements a simple command-line shell in C. The shell can be run in interactive mode or batch mode. In interactive mode, the user can enter commands directly, while in batch mode, commands are read from a specified file.
## Overview
To compile the program, use the provided Makefile. Open a terminal in the same directory as the source code and Makefile and run the following command:
```
make
```
This will create an executable named shell.

### Usage

* Interactive Mode
```
./shell
```
In interactive mode, the shell prompts the user to enter commands. Type commands and press Enter. To exit the shell, enter the command quit.
* Batch Mode
 ```
./shell [batch file]
```
In batch mode, the shell reads commands from the specified batch file. The batch file should contain a list of commands. The shell will execute each command in the order they appear in the file. To exit the shell, include the quit command in the batch file or press Ctrl-D if in interactive mode.

### Command Syntax

The shell supports basic command syntax similar to other Unix-like shells. Commands can be separated by a semicolon (;). Multiple commands on the same line will be executed concurrently.
```
ls -l ; echo "Hello World" ; pwd

```
In this example, the ls -l, echo "Hello World", and pwd commands will run concurrently.

## Error Handling

The program includes basic error handling for invalid input, file opening errors, and command execution errors. If an error occurs, an error message is displayed, and the program exits gracefully.

## Makefile
The Makefile included in the project simplifies the compilation process. The default make command compiles the source code into an executable. To clean up compiled files, use the following command:
```
make clean

```

##Additional Notes

The program uses the execvp() function for command execution.
Please ensure that the program is compiled before running it.
Feel free to customize the shell to add more features or enhance its functionality based on your needs. If you have any questions or encounter issues, please refer to the provided documentation or contact the author.
