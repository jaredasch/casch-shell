#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "parsing.h"

int parent_cmds(char ** args){
    if(!strcmp(args[0], "exit")){ //exit
        exit(0);
        return 1;
    }
    else if(!strcmp(args[0], "cd")){ //CD
        if (chdir(args[1])){
          printf("-casch: %s: --directry not found--\n",args[1]);
        }
        return 1;
    }
    return 0;
}

void parse_redir(char ** args){
  for(int i = 0; args[i]; i++){   // Iterate through args
      if(!strcmp(args[i],">")){
          args[i] = NULL; //replaces > with NULL so execvp doesnt execute > as an arg
          int f = open(args[i+1], O_WRONLY | O_CREAT, 0777);
          dup2(f, 1);
          return;
      }
      if(!strcmp(args[i],">>")){
          args[i] = NULL;
          int f = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0777);
          dup2(f, 1);
          return;
      }
      if(!strcmp(args[i],"<")){

      }
      if(!strcmp(args[i],"|")){

      }
    }
}

void fork_exec(char ** args){
    if (parent_cmds(args)){
      return;
    }
    int f = fork();
    if (f){ //PARENT
        int status, childpid;
        childpid = wait(&status);
    }
    else{ //CHILD
        parse_redir(args);
        if (execvp(args[0], args) == -1){
          printf("-casch: %s: --command not found--\n",args[0]);
          exit(0);
        }
    }
    return;
}

int main(){
    char ** history = calloc(50, 100); //stores up to 100 commands of 50 chars max
    int hist_size = 0;
    char * cmd_buffer = calloc(1, 100);
    while(1){
        char * cwd_buffer = calloc(1, 100);
        getcwd(cwd_buffer, 100);
        printf("%s casch$ ", cwd_buffer);

        fgets(cmd_buffer, 100, stdin);
        history[hist_size] = cmd_buffer;
        hist_size++;

        char ** cmds = parse_line(cmd_buffer);
        for(int i = 0; cmds[i]; i++){   // Iterate through commands
            char ** args = parse_args(cmds[i]);
            fork_exec(args);
        }
        free(cwd_buffer);
    }
    free(history);
    free(cmd_buffer);
    return 0;
}
