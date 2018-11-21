#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

char ** parse_args( char * line ){ //returns array of args (NULL terminated)
    int end = strlen(line) - 1;
    if(line[end] == '\n'){
        line[end] = '\0';
    }

	  char ** arglist = calloc(sizeof(char*), 25); // 25 args max
    for(int i = 0; (arglist[i] = strsep(&line, " ")); i++);
    return arglist;
}

char ** parse_commands(char * cmd_buffer){ //returns array of commands separated by ;
    int end = strlen(cmd_buffer) - 1;
    cmd_buffer[end] = '\0';

    char ** command_list = calloc(sizeof(char**), 100); //100 commands per line max
    for(int i = 0; (command_list[i] = strsep(&cmd_buffer, ";")); i++);
    return command_list;
}

int custom_commands(char ** args){
    if(!strcmp(args[0], "exit")){
        exit(0);
        return 1;
    } else if(!strcmp(args[0], "cd")){
        chdir(args[1]);
        return 1;
    }
    return 0;
}

void fork_exec(char ** args){
    if (custom_commands(args)){
      return;
    }
    int f = fork();
    if (f){ //PARENT
        int status, childpid;
        childpid = wait(&status);
    }
    else{ //CHILD
        if (execvp(args[0], args) == -1){
          printf("--command not found--\n");
          exit(0);
        }
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
