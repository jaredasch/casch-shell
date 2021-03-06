# casch-shell by Colin Hosking and Jared Asch #
Running `make test` runs casch-shell with test commands as input 
## Features ##
* Forks and executes commands!
* Prints out error message when command or file is not found
* Executes multiple commands separated by ;, even if the command is not found
* Redirects using >, >>, <, <<, 2>, 2>>
* Handles single pipes (|)
* Ignores any additional whitespace the user inputs
* Changes working directory using cd
* Prints a funny joke when the user inputs "joke"
* Exits when the user inputs "exit"

## Features Attempted ##
* History
* Tab completion
    * Was planning to implement a tree data structure, which is similar to a tree, except each node is a character, and autocomplete would look at the current node and check for further nodes
    * Deciding what to index was going to be challenging, would need to dynamically index based on the current directory and already entered commands

## Known Bugs ##
* Putting two ;'s next to each other will break the parser, but only if directly next to each other
* When running commands from file with `./out < test_commands`, command prompt still displays multiple times after commands are executed

## Files & Function Headers ##

#### main.c -- handles processes and file management for shell ####
```
/*======== void fork_exec() ==========
	Inputs:  char ** args
	Returns: ---
    Inner Workings:
        First checks if args is not NULL
        Checks if parsing needs to care about pipes
        Checks if the enetered command is one of our custom commands
        Otherwise it forks off another process, parent waits for the child process to exit
	====================*/
```
```
/*======== void parse_redir() ==========
	Inputs:  char ** args
    Returns: A modified version of args with any redirection symbols replaced with NULL
    Inner Workings:
        Checks if each arg is a redirection symbol
        If any indicates redirection, then the appropriate file is duped
	====================*/
```
```
/*======== void parent_cmds() ==========
	Inputs:  char ** args
    Returns: 1 if a custom command is found and run
             0 if no custom command found
    Inner Workings:
        Checks if args[0] is "exit", exits shell if it is
        Checks if args[0] is "cd", changes directory if it is
        Otherwise returns 0
	====================*/
```
```
/*======== void main() ==========
	Inputs: ---
	Returns: 0 if nothing broke
    Inner Workings:
        Prints out current working directory
        Reads stdin into cmd_buffer until a newline is encountered
        Split the cmd_buffer on instances of ";", creating an array of commands cmds
        Executes each command in cmds after splitting it on the whitespace to create an array of arguments
        Repeats
	====================*/
```

#### parsing.c -- handles parsing of commands and arguments ####
```
/*======== char * parse_line() ==========
	Inputs:  char * line
	Returns: An array of command strings
    Inner Workings:
        Seperates a single command by semicolons into individual commands, using strsep(), to be passed to parse_args()
	====================*/
```
```
/*======== char *** parse_pipe() ==========
	Inputs:  char**args
	Returns: An array of command/argument arrays
    Inner Workings:
        Creates two arrays of strings called side1, side2
        Copies all commands/arguments in args before the "|" into side1
        Copies all commands/arguments in args after the "|" into side2
        Creates char*** ans, puts side1 and side2 in ans, and returns ans
	====================*/
```
```
/*======== char ** parse_args() ==========
	Inputs:  char * cmd
	Returns: An array of string arguments of a single command
    Inner Workings:
        Seperates a single command by spaces into arguments, using strsep(), to be passed to execvp()
	====================*/
```

#### piping.c -- handles everything related to piping ####
```
/*======== int ispiped() ==========
	Inputs:  char **args
	Returns: 1 if a "|" (pipe) is found in line
             0 if no pipe is found
    Inner Workings:
        Parses each element in args and checks if it is a pipe
	====================*/
```
```
/*======== void pipe_exec() ==========
	Inputs:  char **args
	Returns: ---
    Inner Workings:
        executes parse_pipe(args), creating pipesides which contains the commands & args on both sides of the "|"
        Creates a pipe in the parent process, then forks
        Child 1 closes the read end of the pipe, redirects STDOUT to the write end of the pipe, then executes side 0 of the pipe
        Child 1 closes the write end of the pipe
        Parent forks again
        Child 2 closes the write end of the pipe, redirects STDIN to the read end of the pipe, then executes side 1 of the pipe
        Child 2 closes the read end of the pipe
        Parent closes both sides of the pipe, then waits until Child 1/2 finishes
	====================*/
```
