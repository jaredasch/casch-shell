#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "parsing.h"

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
          printf("-casch: %s: --command not found--\n",args[0]);
          exit(0);
        }
    }
    return;
}

int main(){
    char * cmd_buffer = calloc(1, 100);
    while(1){
        char * cwd_buffer = calloc(1, 100);
        getcwd(cwd_buffer, 100);
        printf("%s casch$ ", cwd_buffer);
        
        fgets(cmd_buffer, 100, stdin);
        char ** cmds = parse_line(cmd_buffer);
        for(int i = 0; cmds[i]; i++){   // Iterate through commands
            char ** args = parse_args(cmds[i]);
            fork_exec(args);
        }
        free(cwd_buffer);
    }
    free(cmd_buffer);
    return 0;
}
