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

char** parse_redir(char ** args){
  int i = 0;
  int ray[5]; //contains all indicies of <,> etc in args
  int n = -1; //counter for size of ray

  while(args[i]){   // Iterate through args
      if(!strcmp(args[i],">")){
          n++;
          ray[n] = i;
          int f = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
          dup2(f, STDOUT_FILENO);
      }
      if(!strcmp(args[i],">>")){
          n++;
          ray[n] = i;
          int f = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0777);
          dup2(f, STDOUT_FILENO);
      }
      if(!strcmp(args[i],"<")){
          n++;
          ray[n] = i;
          int f = open(args[i+1], O_RDONLY);
          dup2(f, STDIN_FILENO);
      }
      if(!strcmp(args[i],"2>")){
          n++;
          ray[n] = i;
          int f = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
          dup2(f, STDERR_FILENO);
      }
      if(!strcmp(args[i],"2>>")){
          n++;
          ray[n] = i;
          int f = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0777);
          dup2(f, STDERR_FILENO);
      }
      i++;
    }
    while(n>-1){ //sets args[x] to NULL for every x in ray, so execvp doesnt execute <,> ect as an argument
      args[ray[n]] = NULL;
      n--;
    }
    return args;
}

/*======== int ispiped() ==========
	Inputs:  char **args
	Returns: 1 if a "|" (pipe) is found in line
             0 if no pipe is found
    Inner Workings:
        Parses each element in args and checks if it is a pipe
	====================*/
int is_piped(char ** args){ //checks if there is a pipe
  int i = 0;
  while(args[i]){
    if(!strcmp(args[i],"|")){
      return 1;
    }
    i++;
  }
  return 0;
}

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
        Parent closes both sides of the pipe, then waits until Child 2 finishes
	====================*/
void pipe_exec(char ** args){
    char*** pipesides = parse_pipe(args);
    //printf("%s\n", pipesides[0][0]);
    int fds[2];
    pipe(fds);

    int f = fork();
    if (f){
        int f2 = fork();
        if(f2){ //PARENT
          int status;
          close(fds[0]);
          close(fds[1]);
          waitpid(f2,&status, 0);
        }
        else{ //CHILD 2
          close(fds[1]);
          dup2(fds[0], STDIN_FILENO); //dups STDIN to read end of pipe
          execvp(pipesides[1][0], pipesides[1]);
          close(fds[0]);
        }
    }
    else{ //CHILD 1
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO); //dups STDOUT to write end of pipe
        execvp(pipesides[0][0], pipesides[0]);
        close(fds[1]);
    }
    return;
}

/*======== void fork_exec() ==========
	Inputs:  char ** args
	Returns: ---
    Inner Workings:
        First checks if args is not NULL
        Checks if parsing needs to care about pipes
        Checks if the enetered command is one of our custom commands
        Otherwise it forks off another process, parent waits for the child process to exit
	====================*/
void fork_exec(char ** args){
    if (args[0] == NULL){
      return;
    }
    if(is_piped(args)){
      pipe_exec(args);
      return;
    }
    if (parent_cmds(args)){
      return;
    }

    int f = fork();
    if (f){ //PARENT
        int status, childpid;
        childpid = wait(&status);
    }
    else{ //CHILD
        args = parse_redir(args);
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
