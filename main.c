#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

char ** parse_args( char * line ){
    int end = strlen(line) - 1;
    if(line[end] == '\n'){
        line[end] = '\0';
    }
    
	char ** arglist = calloc(sizeof(char*), 25); // 25 args max
	for(int i = 0; (arglist[i] = strsep(&line, " ")); i++);
	return arglist;
}

void custom_commands(char ** args){
    if(!strcmp(args[0], "exit")){
        exit(0);
    } else if(!strcmp(args[0], "exit")){
        chdir(args[1]);
    }
}

void fork_exec(char ** args){
    custom_commands(args);
    int f = fork();
    if (f){ //PARENT 
        int status, childpid;
        childpid = wait(&status);
    }
    else{ //CHILD
        execvp(args[0], args);
    }
    return;
}

int main(){
    char * cmd_buffer = calloc(1, 100);
    while(1){
        printf("casch$ ");
        fgets(cmd_buffer, 100, stdin);
        char ** args = parse_args(cmd_buffer);
        fork_exec(args);
    }
    return 0;
}
