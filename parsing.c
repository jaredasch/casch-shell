#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*======== char * parse_line() ==========
	Inputs:  char * line
	Returns: An array of command strings
    Inner Workings:
        Seperates a single command by semicolons into individual commands, using strsep(), to be passed to parse_args()
	====================*/

char ** parse_line( char * line ){ //returns array of args (NULL terminated)
    int end = strlen(line) - 1;
    line[end] = 0;
	char ** cmd_list = calloc(sizeof(char*), 25); // 25 args max
    for(int i = 0; (cmd_list[i] = strsep(&line, ";")); i++){
        // Remove starting spaces
        while(cmd_list[i] && *cmd_list[i] == ' '){
            cmd_list[i]++;
        }
        // Remove ending spaces
        while(cmd_list[i][ strlen(cmd_list[i])-1 ] == ' '){
            cmd_list[i][ strlen(cmd_list[i])-1 ] = 0;
        }
        // Remove empty commands
        if(*cmd_list[i] == 0) {
            cmd_list[i] = 0;
        }
    };
    return cmd_list;
}

/*======== char *** parse_pipe() ==========
	Inputs:  char**args
	Returns: An array of command/argument arrays
    Inner Workings:
        Creates two arrays of strings called side1, side2
        Copies all commands/arguments in args before the "|" into side1
        Copies all commands/arguments in args after the "|" into side2
        Creates char*** ans, puts side1 and side2 in ans, and returns ans
	====================*/
char *** parse_pipe(char** args){ //breaks args on | into two executable command arrays
  char ** side1 = calloc(sizeof(char*), 25);
  char ** side2 = calloc(sizeof(char*), 25);
  char *** ans = calloc(sizeof(char**),2);
  int i = 0;
  while(strcmp(args[i],"|")){
    side1[i] = args[i];
    i++;
  }
  i++;
  int n = 0;
  while(args[i]){
    side2[n] = args[i];
    i++;
    n++;
  }
  ans[0] = side1;
  ans[1] = side2;
  return ans;
}

/*======== char ** parse_args() ==========
	Inputs:  char * cmd
	Returns: An array of string arguments of a single command
    Inner Workings:
        Seperates a single command by spaces into arguments, using strsep(), to be passed to execvp()
	====================*/
char ** parse_args( char * cmd ){
    char ** arg_list = calloc(sizeof(char*), 25);
    for(int i = 0; (arg_list[i] = strsep(&cmd, " ")); i++){
        // If the arg is an empty string (extra spaces), remove it
        if(*arg_list[i] == 0){
            i--;
        }
    }
    return arg_list;
}
