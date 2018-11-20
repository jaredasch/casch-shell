#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char ** parse_args( char * line ){
    int end = strlen(line) - 1;
    if(line[end] == '\n')
        line[end] = '\0';

	char ** arglist = calloc(sizeof(char*), 25); // 25 args max
	for(int i = 0; (arglist[i] = strsep(&line, " ")); i++);
	return arglist;
}

int main(){
    char * cmd_buffer = calloc(1, 100);
    printf("casch$ ");
    fgets(cmd_buffer, 100, stdin);
    char ** args = parse_args(cmd_buffer);
    execvp(args[0], args);
    return 0;
}
