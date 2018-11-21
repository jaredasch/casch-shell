#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
