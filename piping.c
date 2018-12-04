#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "parsing.h"

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
